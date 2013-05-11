package de.tuberlin.techgi4.gpscalcwebservice;


import java.util.Date;
import java.util.GregorianCalendar;
import java.util.UUID;

import javax.xml.datatype.DatatypeConfigurationException;
import javax.xml.datatype.DatatypeFactory;
import javax.xml.datatype.XMLGregorianCalendar;

public class Client {

	public static void main(String[] args) throws DatatypeConfigurationException {

		GPSCalcImplService service = new GPSCalcImplService();
		GPSCalc gpsCalc = service.getGPSCalcImplPort();
		System.out.println("------->>  Call Started");
		System.out.println(gpsCalc.sayHello("Ali"));
		System.out.println("------->>  Call Ended");
		GpsCoordinates currentGPSCoordinate = new GpsCoordinates();
		// quick hack to convert Date to XMLGregorianCalendar
		GregorianCalendar c = new GregorianCalendar();
		c.setTime(new Date());
		XMLGregorianCalendar date2 = DatatypeFactory.newInstance().newXMLGregorianCalendar(c);

		currentGPSCoordinate.measurementTime=date2;
		currentGPSCoordinate.longitude = 52.512408;
		currentGPSCoordinate.latitude = 13.326924;
		System.out.println("------->>  Call Started");
		UUID myUUID=UUID.randomUUID();
		CoordinateVector coordV=gpsCalc.getDistance(myUUID.toString(),currentGPSCoordinate);
		System.out.println(coordV.distance+":"+coordV.velocity);
		System.out.println("------->>  Call Ended");

		c.setTime(new Date());
		date2 = DatatypeFactory.newInstance().newXMLGregorianCalendar(c);

		date2.setMonth(date2.getMonth()+1);
		currentGPSCoordinate.measurementTime=date2;
		currentGPSCoordinate.longitude = 38.89798;
		currentGPSCoordinate.latitude = -77.036458;
		System.out.println("------->>  Call Started");
		coordV = gpsCalc.getDistance(myUUID.toString(),currentGPSCoordinate);
		System.out.println(coordV.distance+":"+coordV.velocity);
		System.out.println("------->>  Call Ended");
	}

}
