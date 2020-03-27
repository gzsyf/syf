# View more python learning tutorial on my Youtube and Youku channel!!!

# Youtube video tutorial: https://www.youtube.com/channel/UCdyjiB5H8Pu7aDTNVXTTpcg
# Youku video tutorial: http://i.youku.com/pythontutorial

"""
Please note, this code is only for python 3+. If you are using python 2+, please modify the code accordingly.
"""
from __future__ import print_function

from sklearn import datasets								# the sklearn database 
from sklearn.model_selection import train_test_split		# a model ues to make the classification learning 
from sklearn.linear_model import LinearRegression			# a model use to make the regression learning 
from sklearn.neighbors import KNeighborsClassifier
import matplotlib.pyplot as plt								# a library use to draw the picture 
from sklearn import preprocessing							# a library can normalization


###############################################################
#				classification  learning 					  #
###############################################################
iris = datasets.load_iris() # load the data of iris flower 
iris_X = iris.data			# X means iris flower 's characteristic 
iris_y = iris.target		# Y means different kinds of iris flower 

### show the data 
##print(iris_X[:2, :])
##print(iris_y)

# a important function which can separate testdata and trainingdata
# which can help us have a better training 
X_train, X_test, y_train, y_test = train_test_split(
    iris_X, iris_y, test_size=0.3)

##print(y_train)

knn = KNeighborsClassifier()			# instantiate实例化
knn.fit(X_train, y_train)				# fit is the function to train the data 

# compare with trainingdata and realdata 
print(knn.predict(X_test))				# training data 
print(y_test)							# real data 



###############################################################
#				 regression learning线性回归			      #
###############################################################

# fictitious虚构 data to do the experiment of regression learning 
# among of them    X is X-axis Y is Y-axis
X, y = datasets.make_regression(n_samples=100, n_features=1, n_targets=1, noise=10) 

# draw the fictitious data 's axis picture 
plt.scatter(X, y)
plt.show()


model = LinearRegression()  		# instantiate实例化 
model.fit(data_X, data_y)			# fit is the function to train data 

# compare with trainingdata and realdata 
# select First fourt X-axis data and view their Y-axis data 
print(model.predict(data_X[:4, :]))
print(data_y[:4])

print(model.coef_)					# y=ax+b so coef_ is a
print(model.intercept_)				# y=ax+b so intercept_ is b

print(model.get_params())       	# to get the model's parameter 
print(model.score(data_X,data_y))	# to give the train result a score 


###############################################################
#				 normalization			                      #
###############################################################

# fictitious a data to do the normalization experiment 
a = np.array([	[50,  4.5, 100],
				[2.1, 220,   1],
				[33,  2,    66]	],dtype = np.float64)


print(a)
print(preprocessing.scale(a)) 									# it is normalization 
print(preprocessing.minimax_scale(a,feature_range=(-1,1))) 		# it is normalization can shrink to a range 


# this can fictitious a data to do the experiment of classification learning 
X, y = make_classification(n_samples=300, n_features=2 , n_redundant=0, n_informative=2,
                           random_state=22, n_clusters_per_class=1, scale=100)


plt.scatter(X[:, 0], X[:, 1], c=y)	 		# means drae category1 and  category2  c=y means use color to separate  
plt.show()

X = preprocessing.scale(X)    				# normalization 

# a important function which can separate testdata and trainingdata
# which can help us have a better training 
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=.3)

# is the support vector machine 
clf = SVC()									
clf.fit(X_train, y_train)					# use svc to train data 
print(clf.score(X_test, y_test))


###############################################################
#				 cross validation 交叉验证			   		  #
###############################################################

