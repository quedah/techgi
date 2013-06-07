#!coding: utf-8

import sys
import os
import Queue
import threading
import time
from mosquitto import Mosquitto
from xml.dom.minidom import parse, parseString, getDOMImplementation


RED = '\033[31m'
GREEN = '\033[32m'
DEFAULT = '\033[37m'
SUBS = "Gossip"

class VectorClock(dict):
    def __init__(self, name, xmlnode=None):
        self.name = name
        if xmlnode is None:
            self[name] = 0
        else:
            nodes = xmlnode.getElementsByTagName('Process')
            for node in nodes:
                name = node.getElementsByTagName('Name')[0].firstChild.nodeValue
                t = node.getElementsByTagName('Time')[0].firstChild.nodeValue
                self[name] = int(t)

    def __lt__(self, other):
        """
        Check if all entries in self are <= and at least one entry strictly <
        than in other.
        """
        other_names = other.keys()
        own_names = self.keys()
        diff_count = 0      # Count of elements strictly smaller in self.
        for name in own_names:
            if name not in other_names:
                return False
            elif self[name] > other[name]:
                return False
            elif self[name] < other[name]:
                diff_count += 1

        if diff_count > 0:
            return True
        else:
            return False

    def __gt__(self,other):
        """
        Check if all entries in self are >= and at least one entry strictly >
        than in other.
        """
        return other < self

    def increment(self):
        """
        Increment own time by 1.
        """
        self[self.name] += int(1)

    def toxml(self, xmldoc ):
        root = xmldoc.createElement('VC')

        for name, value in self.items():
            node = xmldoc.createElement("Process")

            element = xmldoc.createElement('Name')
            element.appendChild(xmldoc.createTextNode(str(name)))
            node.appendChild(element)

            element = xmldoc.createElement('Time')
            element.appendChild(xmldoc.createTextNode(str(value)))
            node.appendChild(element)

            root.appendChild(node)

        xmldoc.documentElement.appendChild(root)

    def sync(self, other):
        """
        Set own clock to max of other clock.
        """
        other_names = other.keys()
        own_names = self.keys()
        for name in other_names:
            if name not in own_names:  self[name] = int(other[name])
            elif self[name] < other[name]:  self[name] = int(other[name])

class Client(Mosquitto,object):
    def __init__(self,host, port, name=None):
        self.name = name
        self.host = host
        self.port = port

        if name is None:    self.name = os.getpid()
        else:               self.name = name

        super(Client,self).__init__(name)
        self.on_message = self.unpack

        self.connect(self.host, self.port, 60)
        self.subscribe(SUBS,0)

        self.vc = VectorClock(self.name)            # VC for sending data.
        self.print_vc = VectorClock(self.name)      # VC of last printed msg.
        self.stash = [] # Store out-of-sync messages here until printed.

    def run(self):
        """
        Run the client, i.e. check server for new posts and listen for keyboard
        input.
        """
        q = Queue.Queue()
        t = threading.Thread(target=self.receive)
        t2 = threading.Thread(target=self.check_stash)
        t.daemon = True
        t2.daemon = True
        t.start()
        t2.start()

        self.talk()

    def receive(self):
        """
        Method called by 'run' to check for new messages.
        """
        rc = 0
        while rc == 0:
            rc = self.loop()

        print "rc:  ",rc
        self.disconnect()

    def talk(self):
        """
        Method called by 'run' to listen to keyboard input and publish.
        """
        while True:
            txt = raw_input("\n"+GREEN+"--->    "+DEFAULT)
            if len(txt) > 0:
                if txt == "exit": 
                    print "\n\nBye {0}, Have A Nice Day!\n".format(self.name)
                    time.sleep(1)
                    sys.exit()
                self.vc.increment()
                self.print_vc.increment()
                txt = self.pack(txt)
                self.publish("Gossip",txt,0)

    def pack(self, msg):
        dom = getDOMImplementation()
        xmldoc = dom.createDocument(None, "Mule", None)
        doc_root = xmldoc.documentElement

        message = xmldoc.createElement('Message')
        message.appendChild(xmldoc.createTextNode(self.name))
        doc_root.appendChild(message)

        element = xmldoc.createElement('Name')
        element.appendChild(xmldoc.createTextNode(self.name))
        message.appendChild(element)

        element = xmldoc.createElement('Payload')
        element.appendChild(xmldoc.createTextNode(str(msg)))
        message.appendChild(element)

        self.vc.toxml(xmldoc)

        return str(doc_root.toxml())

    @staticmethod
    def unpack(mqtt, obj, msg):
        dom = parseString(msg.payload)

        # Get Name
        name = dom.getElementsByTagName('Name')
        if len(name) > 0: 
            name = name[0].firstChild.nodeValue
        else:   
            return None         # Invalid data format.

        if name == mqtt.name: return None   # Own message. Ignore.
        sync_data = dom.getElementsByTagName('Sync')
        if len(sync_data) > 0:
            pass

        # Get Vector Clock
        vc_dom = dom.getElementsByTagName('VC')
        message = dom.getElementsByTagName('Message')
        if len(vc_dom) == 0:    return None     # No attached VC. 
        if len(message) == 0:   return None     # No attached message.

        # Retrieve data.
        other_vc = VectorClock(name,xmlnode=vc_dom[0])
        payload = dom.getElementsByTagName('Payload')[0].firstChild.nodeValue
        message = message[0]

        mqtt.vc.sync(other_vc)

        if mqtt.printable(other_vc):    
            mqtt.print_msg(other_vc, payload)
        else:           
            mqtt.stash.append((other_vc, payload)) 

    def check_stash(self):
        while True:
            time.sleep(1)
            n = len(self.stash) - 1
            for i, (other_vc, msg) in enumerate(reversed(self.stash)):
                if self.printable(other_vc):
                    self.print_msg(other_vc, msg)
                    self.stash.pop(n - i)   # Remove msg from stash.

    def print_msg(self, other_vc, payload):
        name = other_vc.name
        print "\r\r\r\r\r\r\r\r"+RED+name+10*" "+"\n<---"+DEFAULT,
        print "    {0}".format(payload)
        print "\n"+GREEN+"--->    "+DEFAULT,
        sys.stdout.flush()
        self.print_vc.sync(other_vc)

    def printable(self, other_vc):
        """
        Checks if this message can be printed as is. Returns False if message
        has to be stashed, i.e. there is some other unprinted message that comes
        first. Else returns True.
        """
        #return True
        other_names = other_vc.keys()
        own_names = self.print_vc.keys()

        for name in other_names:
            if name in own_names:
                # Sender can be incremented by up to +1.
                if name == other_vc.name:
                    if other_vc[name] > self.print_vc[name] + 1:
                        #print "SENDER: ", name, ": ", other_vc
                        #print "SELF: ", self.print_vc
                        return False
                # Everyone else must have values smaller or equal.
                elif other_vc[name] > self.print_vc[name]:
                    #print "SUBJECT: ", name, ": ", other_vc
                    #print "SELF: ", self.print_vc
                    return False

        return True


def main():
    args = sys.argv
    if len(args) < 3:   sys.exit("usage: ./client host port [id]")
    else:               
        HOST = str(args[1])
        PORT = int(args[2])

    if len(args) == 4:  name = str(args[3])
    else:   name = None

    client = Client(HOST, PORT, name)
    client.run()

if __name__ == "__main__":
    main()
