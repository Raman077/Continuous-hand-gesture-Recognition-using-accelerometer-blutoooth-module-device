from sklearn import datasets
from sklearn import svm
from sklearn.model_selection import train_test_split
from sklearn.metrics import confusion_matrix
import scipy as sp
import os, signals
from sklearn.externals import joblib
from sklearn.model_selection import GridSearchCV


if __name__ == '__main__':
	
	SHOW_CONFUSION_MATRIX = False

	x_data = []
	y_data = []

	classes = {}

	root="C:\Users\shubh_000\Desktop\proyecto\gesture-keyboard-master\data" #Default directory containing the dataset

	print "Loading the dataset from '{directory}'...".format(directory=root),

	
	for path, subdirs, files in os.walk(root):
		for name in files:
			filename = os.path.join(path, name)
			sample = signals.Sample.load_from_file(filename)
			
			category = name.split("_")[0]
			
			
			if len(category) >0:
				number = ord(category) - ord("a")
				y_data.append(number)
				classes[number] = category
				x_data.append(sample.get_linearized())

	print "DONE"

	params = {'C':[0.001,0.01,0.1,1], 'kernel':['linear']}

	
	svc = svm.SVC(probability = True)
	clf = GridSearchCV(svc, params,verbose =10, n_jobs=8)

	X_train, X_test, Y_train, Y_test = train_test_split(x_data, 
				y_data, test_size=0.35, random_state=0)

	print "Starting the training process..."

	
	clf.fit(X_train, Y_train)

	
	if SHOW_CONFUSION_MATRIX:
		print "Confusion Matrix:"
		Y_predicted = clf.predict(X_test)
		print confusion_matrix(Y_test, Y_predicted)
	
	print "\nBest estimator parameters: "
	print clf.best_estimator_
	
	
	score = clf.score(X_test, Y_test)

	print "\nSCORE: {score}\n".format(score = score)

	print "Saving the model...",

	joblib.dump(clf, 'model.pkl') 
	joblib.dump(classes, 'classes.pkl') 

	print "DONE"
