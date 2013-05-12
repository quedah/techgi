package de.tuberlin.techgi4.gpscalcwebservice;


import java.util.ArrayList;
import java.util.Date;
import java.util.GregorianCalendar;
import java.util.List;
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
		
		UUID client1ID=UUID.randomUUID();
		UUID client2ID=UUID.randomUUID();
		System.out.println("Client1 has id:"+client1ID);
		System.out.println("Client2 has id:"+client2ID);
		
		// Client 1 Step 1
		// quick hack to convert Date to XMLGregorianCalendar
		GregorianCalendar c = new GregorianCalendar();
		c.setTime(new Date());
		XMLGregorianCalendar date2 = DatatypeFactory.newInstance().newXMLGregorianCalendar(c);

		
		
		currentGPSCoordinate.measurementTime=date2;
		currentGPSCoordinate.longitude = 52.512408;
		currentGPSCoordinate.latitude = 13.326924;
		System.out.println("------->>  1 Call Started client 1 started from TU Berlin");
		
		CoordinateVector coordV=gpsCalc.getDistance(client1ID.toString(),currentGPSCoordinate);
		System.out.println(coordV.distance+":"+coordV.velocity);
		System.out.println("------->>  1 Call Ended");

			// Client 2 Step 2
		currentGPSCoordinate.measurementTime=date2;
		currentGPSCoordinate.longitude = 38.89798;
		currentGPSCoordinate.latitude = -77.036458;
		System.out.println("------->> 2 Call Started client 2 appears on white house");
		coordV = gpsCalc.getDistance(client2ID.toString(),currentGPSCoordinate);
		System.out.println(coordV.distance+":"+coordV.velocity);
			System.out.println("------->> 2 Call Ended");
			
			// Client 1 Step 2
		c.setTime(new Date());
		date2 = DatatypeFactory.newInstance().newXMLGregorianCalendar(c);

		date2.setMonth(date2.getMonth()+1);
		currentGPSCoordinate.measurementTime=date2;
		currentGPSCoordinate.longitude = 38.89798;
		currentGPSCoordinate.latitude = -77.036458;
		System.out.println("------->> 3 Call Started client 1 move to white house");
		coordV = gpsCalc.getDistance(client1ID.toString(),currentGPSCoordinate);
		System.out.println(coordV.distance+":"+coordV.velocity);
		List<String> newNeighbours=gpsCalc.getOtherClientIdin(client1ID.toString(),coordV.velocity);
		for(String neighbour : newNeighbours)
		{
			System.out.println("new neighbour has id:"+neighbour);
		}
		System.out.println("------->> 3 Call Ended");
		
		// Client 1 Step 2
		System.out.println("------->> 4 Call Started client 1 move to white house");
	 newNeighbours=gpsCalc.getOtherClientIdin(client2ID.toString(),0);
		for(String neighbour : newNeighbours)
		{
			System.out.println("You have met:"+neighbour);
		}
		System.out.println("------->> 4 Call Ended");



	
	}

}
