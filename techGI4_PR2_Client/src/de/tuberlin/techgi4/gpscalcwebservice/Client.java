package de.tuberlin.techgi4.gpscalcwebservice;

public class Client {

		public static void main(String[] args){
			
			GPSCalcImplService service = new GPSCalcImplService();
			GPSCalc gpsCalcGreeting = service.getGPSCalcImplPort(); 
			System.out.println("------->>  Call Started");
			System.out.println(gpsCalcGreeting.sayHello("Ali"));
			System.out.println("------->>  Call Ended");
				}
}
