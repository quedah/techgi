package de.tuberlin.techgi4.gpscalcWebservice;

import java.util.ArrayList;
import java.util.UUID;

import javax.jws.WebMethod;
import javax.jws.WebService;

@WebService
public interface GPSCalc {
	@WebMethod String sayHello(String name);
	@WebMethod CoordinateVector getDistance(UUID clientID, GPSCoordinates coordinates);
	@WebMethod ArrayList<UUID> getOtherClientIdin(UUID clientID, double velocity);

}
