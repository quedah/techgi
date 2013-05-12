package de.tuberlin.techgi4.gpsandroidclient;

import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.Menu;
import org.ksoap2.SoapEnvelope;
import org.ksoap2.serialization.SoapObject;
import org.ksoap2.serialization.SoapPrimitive;
import org.ksoap2.serialization.SoapSerializationEnvelope;
import org.ksoap2.transport.HttpTransportSE;
import org.ksoap2.serialization.PropertyInfo;


import de.tuberlin.techgi4.gpsandroidclient.R;

public class MainActivity extends Activity {
	private static final String NAMESPACE = "http://gpscalcWebservice.techgi4.tuberlin.de/";
	private static String URL = "http://localhost:8080/WS/Greeting?wsdl";
	private static final String METHOD_NAME = "hello";
	private static final String SOAP_ACTION = "http://localhost:8080/WS/Greeting";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		SoapObject request = new SoapObject(NAMESPACE, METHOD_NAME);
		PropertyInfo propInfo = new PropertyInfo();
		propInfo.name = "arg0";
		propInfo.type = PropertyInfo.STRING_CLASS;

		request.addProperty(propInfo, "John Smith");

		SoapSerializationEnvelope envelope = new SoapSerializationEnvelope(
				SoapEnvelope.VER11);

		envelope.setOutputSoapObject(request);
		HttpTransportSE androidHttpTransport = new HttpTransportSE(URL);

		try {
			androidHttpTransport.call(SOAP_ACTION, envelope);

			SoapPrimitive resultsRequestSOAP = (SoapPrimitive) envelope
					.getResponse();

			System.out.println(resultsRequestSOAP.toString());

		} catch (Exception e) {
			Log.e("soap", "exp" );e.printStackTrace();
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}

}
