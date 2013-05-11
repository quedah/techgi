package de.tuberlin.techgi4.gpscalcWebservice;

import javax.jws.WebService;


@WebService(endpointInterface = "de.tuberlin.techgi4.gpscalcWebservice.GPSCalc")
public class GPSCalcImpl implements GPSCalc {

	@Override
	public
	String sayHello(String name) {
		return "Hello, Welcom to jax-ws " + name;
	}

}
