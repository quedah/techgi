package de.tuberlin.techgi4.gpscalcWebservice;

import java.util.Date;

public class GPSCoordinates {
	public Date measurementTime;
	public double longitude;
	public double latitude;

	public GPSCoordinates() {
		super();
	}

	public GPSCoordinates(double lontitude, double latitude,
			Date measurementTime) {
		super();
		this.longitude = lontitude;
		this.latitude = latitude;
		this.measurementTime = measurementTime;
	}

	public double calcDistanceTo(GPSCoordinates otherCoordinate) {
		return Math
				.sqrt(Math.pow(
						Math.abs(this.latitude - otherCoordinate.latitude), 2)
						+ Math.pow(
								Math.abs(this.longitude
										- otherCoordinate.longitude), 2));
	}

	public double calcVelocityFrom(double distance, Date timeAfter) {
		return distance/(timeAfter.getTime()/1000-this.measurementTime.getTime()/1000);
	}

}
