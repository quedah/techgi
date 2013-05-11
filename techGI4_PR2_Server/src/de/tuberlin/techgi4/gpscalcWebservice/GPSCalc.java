package de.tuberlin.techgi4.gpscalcWebservice;
import javax.jws.WebMethod;
import javax.jws.WebService;

@WebService
public interface GPSCalc {
	@WebMethod String sayHello(String name);
}
