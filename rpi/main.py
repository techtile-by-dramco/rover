from flask import Flask, request
import json
from flask_cors import CORS, cross_origin

import serial
import time
import numpy as np
from Sensorcombiner import Sensorcombiner
from Hat import Hat
from Encoder import Encoder
from Motordriver import Motordriver


app = Flask(__name__)
CORS(app)


#===========================================
#===           ***  Posts ***            ===
#===========================================

@app.route("/post_motor_speed_json", methods=['POST'])
def speed():
    content_type = request.headers.get('Content-Type')
    if (content_type == 'application/json'):
        json = request.json
        print(json["dir_1"])
        return json
    else:
        return 'Content-Type not supported!'


# Distance sensor
@app.route("/start_scanning", methods=['POST'])
def start_scanning():
    sc.start_scanning()
    return "ok"

# Distance sensor
@app.route("/stop_scanning", methods=['POST'])
def stop_scanning():
    sc.stop_scanning()
    return "ok"

# Distance sensor
@app.route("/change_update_rate", methods=['POST'])
def change_update_rate():
    sc.change_update_rate(1)
    return "ok"


#===========================================
#===          ***  Requests ***          ===
#===========================================

@app.route("/get_rpm")
def rpm():
    data_set = {"rpm1": ec.get_rpm(0), "rpm2": ec.get_rpm(1), "rpm3": ec.get_rpm(2)}
    json_dump = json.loads(json.dumps(data_set))
    return json_dump


@app.route("/get_distance")
def distance():
    data = sc.read_distance_buffer()
    data_set = {"distance": data}
    json_dump = json.loads(json.dumps(data_set))
    return json_dump


#===========================================
#===            ***  Main ***            ===
#===========================================

def main():
    while 1:
        time.sleep(1)
        a = sc.read_distance_buffer()
        print(a)
        print(a[0])



#===========================================
#===            ***  Main ***            ===
#===========================================

if __name__== "__main__":

    port = serial.Serial('/dev/ttyS0', 2000000, timeout=1)
    sc = Sensorcombiner(port)

    sc.start_scanning()
    #sc.stop_scanning()
    
    hat = Hat()

    ec = Encoder()

    driver = Motordriver(hat, ec)

    # main()
    
    app.run(host="0.0.0.0") 




