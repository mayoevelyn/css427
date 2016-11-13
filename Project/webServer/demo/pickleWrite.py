import pickle

shared = {"Foo":"Bar", "Honey":"Badger", "Shit":"Storm"}
fp = open("shared.pkl", "w")
pickle.dump(shared, fp)
