package de.tuberlin.techgi4.gpscalcWebservice;

import java.util.UUID;

public class ClientCoordinates {
	public UUID identifier;
	public GPSCoordinates coordinates;
	public ClientCoordinates(UUID identifier, GPSCoordinates coordinates) {
		super();
		this.identifier = identifier;
		this.coordinates = coordinates;
	}


}