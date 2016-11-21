import pickle
from collections import defaultdict

currentAverages = {"Temperature":"35C", "Humidity":"10%", "Brightness":"120L", "Moisture":"2%"}

valveState = {"ValveState":"Open"}

history = defaultdict(dict)

history["11/01/2016 AM"]["Date"] = "11/01/2016 AM"
history["11/01/2016 AM"]["Irrigated"] = "IRRIGATED"
history["11/01/2016 AM"]["Reasons"] = "reason1, reason2, reason3"
history["11/01/2016 AM"]["DailyAverages"] = "xC, x%, xL, x%"

history["11/01/2016 PM"]["Date"] = "11/01/2016 PM"
history["11/01/2016 PM"]["Irrigated"] = "SKIPPED"
history["11/01/2016 PM"]["Reasons"] = "reason4, reason5, reason6"
history["11/01/2016 PM"]["DailyAverages"] = "xC, x%, xL, x%"

sensors = defaultdict(dict)

sensors["Temperature"]["Reading"] = 25
sensors["Temperature"]["Threshold"] = 20
sensors["Humidity"]["Reading"] = 60
sensors["Humidity"]["Threshold"] = 20
sensors["Brightness"]["Reading"] = 120
sensors["Brightness"]["Threshold"] = 90
sensors["Moisture"]["Reading"] = 10
sensors["Moisture"]["Threshold"] = 5


fp = open("averages.pkl", "w")
pickle.dump(currentAverages, fp)

fp = open("valveState.pkl", "w")
pickle.dump(valveState, fp)

fp = open("history.pkl", "w")
pickle.dump(history, fp)

fp = open("sensors.pkl", "w")
pickle.dump(sensors, fp)
