import pickle
from collections import defaultdict



fp = open("formData/formValveState.pkl")
formValveState = pickle.load(fp)

if formValveState["ValveState"] == "Open":    
    valveState = {"ValveState":"Open"}
else:
    valveState = {"ValveState":"Closed"}

currentAverages = {"Temperature":"35C", "Humidity":"10%", "Brightness":"120L", "Moisture":"2%"}

history = defaultdict(dict)

history["11/01/2016 AM"]["Date"] = "11/01/2016 AM"
history["11/01/2016 AM"]["Irrigated"] = "IRRIGATED"
history["11/01/2016 AM"]["Reasons"] = "reason1, reason2, reason3"
history["11/01/2016 AM"]["DailyAverages"] = "xC, x%, xL, x%"

history["11/01/2016 PM"]["Date"] = "11/01/2016 PM"
history["11/01/2016 PM"]["Irrigated"] = "SKIPPED"
history["11/01/2016 PM"]["Reasons"] = "reason4, reason5, reason6"
history["11/01/2016 PM"]["DailyAverages"] = "xC, x%, xL, x%"

sensorData = defaultdict(dict)

sensorData["Temperature"]["Reading"] = 25
sensorData["Temperature"]["Threshold"] = 20
sensorData["Humidity"]["Reading"] = 60
sensorData["Humidity"]["Threshold"] = 20
sensorData["Brightness"]["Reading"] = 120
sensorData["Brightness"]["Threshold"] = 90
sensorData["Moisture"]["Reading"] = 10
sensorData["Moisture"]["Threshold"] = 5




fp = open("cmdStationData/averages.pkl", "w")
pickle.dump(currentAverages, fp)

fp = open("cmdStationData/valveState.pkl", "w")
pickle.dump(valveState, fp)

fp = open("cmdStationData/history.pkl", "w")
pickle.dump(history, fp)

fp = open("cmdStationData/sensorData.pkl", "w")
pickle.dump(sensorData, fp)


###########################
# OMIT FROM PRODUCTION CODE
###########################

valveSeqNum = {}
valveSeqNum["SeqNum"] = 0

sensorsSeqNum = {}
sensorsSeqNum["SeqNum"] = 0

preferencesSeqNum = {}
preferencesSeqNum["SeqNum"] = 0

fp = open("sequenceNumbers/formValveSeqNum.pkl", "w")
pickle.dump(valveSeqNum, fp)

fp = open("sequenceNumbers/formSensorsSeqNum.pkl", "w")
pickle.dump(sensorsSeqNum, fp)

fp = open("sequenceNumbers/formPreferencesSeqNum.pkl", "w")
pickle.dump(preferencesSeqNum, fp)
