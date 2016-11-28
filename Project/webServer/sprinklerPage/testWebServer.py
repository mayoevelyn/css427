import pickle
import time
from threading import Thread

valveSeqNum = 1
sensorsSeqNum = 1

while True:
    time.sleep(0.25)
    
    # Valve toggle
    
    try:
        fp = open("formData/formValveState.pkl")
        formValveState = pickle.load(fp)
    
        if valveSeqNum <= formValveState["SeqNum"]:
            print("\n" + str(valveSeqNum) + " Valve State: " + formValveState["ValveState"])
            valveSeqNum = valveSeqNum + 1

        if formValveState["SeqNum"] == 0 and valveSeqNum == 10:
            valveSeqNum = 0           
            
    except:
        print "valve exception"


    # Sensors
    try:
        fp = open("formData/formSensors.pkl")
        formSensors = pickle.load(fp)
    
        if sensorsSeqNum <= formSensors["SeqNum"]:
            print("\n" + str(sensorsSeqNum) + " Sensor Thresholds")

            try:
                tThresh = int(formSensors["TemperatureThreshold"])
                print("Temperature Threshold: " + str(tThresh))
            except:
                pass

            try:
                hThresh = int(formSensors["HumidityThreshold"])
                print("Humidity Threshold: " + str(hThresh))
            except:
                pass

            try:
                bThresh = int(formSensors["BrightnessThreshold"])
                print("Brightness Threshold: " + str(bThresh))
            except:
                pass

            try:
                mThresh = int(formSensors["MoistureThreshold"])
                print("Moisture Threshold: " + str(mThresh))
            except:
                pass

            sensorsSeqNum = sensorsSeqNum + 1

        if formSensors["SeqNum"] == 0 and sensorsSeqNum == 10:
            sensorsSeqNum = 0           
            
    except:
        print "valve exception"
        
    
    

    

