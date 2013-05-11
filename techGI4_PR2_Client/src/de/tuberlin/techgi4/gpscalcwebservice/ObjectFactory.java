
package de.tuberlin.techgi4.gpscalcwebservice;

import javax.xml.bind.JAXBElement;
import javax.xml.bind.annotation.XmlElementDecl;
import javax.xml.bind.annotation.XmlRegistry;
import javax.xml.namespace.QName;


/**
 * This object contains factory methods for each 
 * Java content interface and Java element interface 
 * generated in the de.tuberlin.techgi4.gpscalcwebservice package. 
 * <p>An ObjectFactory allows you to programatically 
 * construct new instances of the Java representation 
 * for XML content. The Java representation of XML 
 * content can consist of schema derived interfaces 
 * and classes representing the binding of schema 
 * type definitions, element declarations and model 
 * groups.  Factory methods for each of these are 
 * provided in this class.
 * 
 */
@XmlRegistry
public class ObjectFactory {

    private final static QName _SayHello_QNAME = new QName("http://gpscalcWebservice.techgi4.tuberlin.de/", "sayHello");
    private final static QName _SayHelloResponse_QNAME = new QName("http://gpscalcWebservice.techgi4.tuberlin.de/", "sayHelloResponse");
    private final static QName _GetDistanceResponse_QNAME = new QName("http://gpscalcWebservice.techgi4.tuberlin.de/", "getDistanceResponse");
    private final static QName _GetOtherClientIdin_QNAME = new QName("http://gpscalcWebservice.techgi4.tuberlin.de/", "getOtherClientIdin");
    private final static QName _GetOtherClientIdinResponse_QNAME = new QName("http://gpscalcWebservice.techgi4.tuberlin.de/", "getOtherClientIdinResponse");
    private final static QName _GetDistance_QNAME = new QName("http://gpscalcWebservice.techgi4.tuberlin.de/", "getDistance");

    /**
     * Create a new ObjectFactory that can be used to create new instances of schema derived classes for package: de.tuberlin.techgi4.gpscalcwebservice
     * 
     */
    public ObjectFactory() {
    }

    /**
     * Create an instance of {@link GetDistance }
     * 
     */
    public GetDistance createGetDistance() {
        return new GetDistance();
    }

    /**
     * Create an instance of {@link GetOtherClientIdinResponse }
     * 
     */
    public GetOtherClientIdinResponse createGetOtherClientIdinResponse() {
        return new GetOtherClientIdinResponse();
    }

    /**
     * Create an instance of {@link GetOtherClientIdin }
     * 
     */
    public GetOtherClientIdin createGetOtherClientIdin() {
        return new GetOtherClientIdin();
    }

    /**
     * Create an instance of {@link GetDistanceResponse }
     * 
     */
    public GetDistanceResponse createGetDistanceResponse() {
        return new GetDistanceResponse();
    }

    /**
     * Create an instance of {@link SayHelloResponse }
     * 
     */
    public SayHelloResponse createSayHelloResponse() {
        return new SayHelloResponse();
    }

    /**
     * Create an instance of {@link SayHello }
     * 
     */
    public SayHello createSayHello() {
        return new SayHello();
    }

    /**
     * Create an instance of {@link CoordinateVector }
     * 
     */
    public CoordinateVector createCoordinateVector() {
        return new CoordinateVector();
    }

    /**
     * Create an instance of {@link GpsCoordinates }
     * 
     */
    public GpsCoordinates createGpsCoordinates() {
        return new GpsCoordinates();
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link SayHello }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://gpscalcWebservice.techgi4.tuberlin.de/", name = "sayHello")
    public JAXBElement<SayHello> createSayHello(SayHello value) {
        return new JAXBElement<SayHello>(_SayHello_QNAME, SayHello.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link SayHelloResponse }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://gpscalcWebservice.techgi4.tuberlin.de/", name = "sayHelloResponse")
    public JAXBElement<SayHelloResponse> createSayHelloResponse(SayHelloResponse value) {
        return new JAXBElement<SayHelloResponse>(_SayHelloResponse_QNAME, SayHelloResponse.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link GetDistanceResponse }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://gpscalcWebservice.techgi4.tuberlin.de/", name = "getDistanceResponse")
    public JAXBElement<GetDistanceResponse> createGetDistanceResponse(GetDistanceResponse value) {
        return new JAXBElement<GetDistanceResponse>(_GetDistanceResponse_QNAME, GetDistanceResponse.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link GetOtherClientIdin }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://gpscalcWebservice.techgi4.tuberlin.de/", name = "getOtherClientIdin")
    public JAXBElement<GetOtherClientIdin> createGetOtherClientIdin(GetOtherClientIdin value) {
        return new JAXBElement<GetOtherClientIdin>(_GetOtherClientIdin_QNAME, GetOtherClientIdin.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link GetOtherClientIdinResponse }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://gpscalcWebservice.techgi4.tuberlin.de/", name = "getOtherClientIdinResponse")
    public JAXBElement<GetOtherClientIdinResponse> createGetOtherClientIdinResponse(GetOtherClientIdinResponse value) {
        return new JAXBElement<GetOtherClientIdinResponse>(_GetOtherClientIdinResponse_QNAME, GetOtherClientIdinResponse.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link GetDistance }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://gpscalcWebservice.techgi4.tuberlin.de/", name = "getDistance")
    public JAXBElement<GetDistance> createGetDistance(GetDistance value) {
        return new JAXBElement<GetDistance>(_GetDistance_QNAME, GetDistance.class, null, value);
    }

}
