package de.tuberlin.techgi4.gpscalcWebservice;

import java.util.ArrayList;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.UUID;

import javax.jws.WebMethod;
import javax.jws.WebService;

@WebService(endpointInterface = "de.tuberlin.techgi4.gpscalcWebservice.GPSCalc")
public class GPSCalcImpl implements GPSCalc {
	Hashtable<UUID, GPSCoordinates> clientLocalizationMap;

	public GPSCalcImpl() {
		super();
		this.clientLocalizationMap = new Hashtable<UUID, GPSCoordinates>();
	}

	@Override
	public String sayHello(String name) {
		return "Hello, Welcom to jax-ws " + name;
	}

	@Override
	@WebMethod
	public CoordinateVector getDistance(UUID clientID,
			GPSCoordinates currentCoordinates) {
		double distance = Double.NaN;
		double velocity = Double.NaN;
		// get old coordinates of client
		GPSCoordinates oldCoordinates = clientLocalizationMap.get(clientID);

		// no old data is stored for client
		if (oldCoordinates == null) {
			distance = Double.NaN;
			velocity = Double.NaN;
		} else {
			// calculate distance
			distance = oldCoordinates.calcDistanceTo(currentCoordinates);
			// calculate velocity
			velocity = oldCoordinates.calcVelocityFrom(distance,
					currentCoordinates.measurementTime);
			// remove old value
			clientLocalizationMap.remove(clientID);
		}
		// store new Coordinates
		clientLocalizationMap.put(clientID, currentCoordinates);

		return new CoordinateVector(distance, velocity);
	}

	@Override
	@WebMethod
	public ArrayList<UUID> getOtherClientIdin(UUID clientID, double velocity) {
		// other clients near by
		ArrayList<UUID> clientsSourround = new ArrayList<UUID>();
		// get coordinates of client
		GPSCoordinates clientCoordinates = clientLocalizationMap.get(clientID);
		Enumeration<UUID> keyEnum = clientLocalizationMap.keys();
		while (keyEnum.hasMoreElements()) {
			UUID currentClientID = keyEnum.nextElement();
			if (clientLocalizationMap.get(currentClientID).isInRadius(velocity,
					clientCoordinates)) {
				clientsSourround.add(currentClientID);
			}

		}

		return clientsSourround;

	}

}
