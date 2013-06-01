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
                self[name] = t

    def increment(self):
        """
        Increment own time by 1.
        """
        self[self.name] += 1

    def toxml(self, xmldoc ):
        root = xmldoc.createElement('VC')

        print self
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
        Checks if this and other are properly synced. If there are missing
        messages returns False and only entry for 'other' is incremented. If
        there are no missing messages returns True and everything is
        incremented.
        """
        self[other.name] = other[other.name]
        other_names = other.keys()
        self_names = self.keys()
        if self.name in other_names:    other_names.remove(self.name)
        other_names.remove(other.name)

        for name in other_names:
            if name not in self_names:
                self[name] = other[name]        # Add prev. unknown name.
            elif self[name] < other[name]:
                return False                    # Missed s.th., stash for now.

        return True                             # Everything OK.

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
        self.subscribe(SUBS,1)

        self.vc = VectorClock(self.name)
        self.stash = [] # Store out-of-sync messages here until printed.

    def run(self):
        """
        Run the client, i.e. check server for new posts and listen for keyboard
        input.
        """
        q = Queue.Queue()
        t = threading.Thread(target=self.receive)
        t.daemon = True
        t.start()

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
                txt = self.pack(txt)
                self.publish("Gossip",txt,1)

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
        if len(vc_dom) > 0:
            # Received legitimate message.
            mqtt.vc.increment()
            other_vc = VectorClock(name,xmlnode=vc_dom[0])
            in_sync = mqtt.vc.sync(other_vc)
            n = len(mqtt.stash) - 1
            for i, stashed_msg in enumerate(reversed(mqtt.stash)):
                if mqtt.vc.sync(stashed_msg[0]):
                    mqtt.print_msg(stashed_msg[1])
                    mqtt.stash.pop(n - i)   # Remove msg from stash.
        else:
            return None         # Abort, invalid data.

        # Get Message
        message = dom.getElementsByTagName('Message')
        if len(message) > 0:
            message = message[0]
            payload = dom.getElementsByTagName('Payload')[0].firstChild.nodeValue
            if in_sync:     mqtt.print_msg(name, payload)
            else:           stash.append((other_vc, payload)) 


    def print_msg(self, name, payload):
        print "\r\r\r\r\r\r\r\r"+RED+name+10*" "+"\n<---"+DEFAULT,
        print "    {0}".format(payload)
        print "\n"+GREEN+"--->    "+DEFAULT,
        sys.stdout.flush()

def main():
    vc = VectorClock("Ed")
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
