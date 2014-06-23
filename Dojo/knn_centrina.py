
import time
import numpy as np

def read_file(filename):
	with open(filename,'r') as v:
		v.readline()
		values = []
		for line in v:
			xs = [int(x) for x in line.split(',')]
			label = xs[0]
			xs = np.array(xs[1:],dtype=np.int32)
			values += [(label,xs)]
	return values

validation_sample = read_file('validationsample.csv')
testing_sample = read_file('trainingsample.csv')


start_time = time.time()


vmat = np.zeros((len(validation_sample),len(validation_sample[0][1])))
tmat = np.zeros((len(testing_sample[0][1]),len(testing_sample)))

for i,(_,xs) in enumerate(validation_sample):
	vmat[i,:] = xs

for i,(_,ys) in enumerate(testing_sample):
	tmat[:,i] = ys

abmat = -2*np.dot(vmat,tmat)
for i,(x_label,xs) in enumerate(validation_sample):
	abmat[i,:] += np.sum(xs*xs)
for j,(y_label,ys) in enumerate(testing_sample):
	abmat[:,j] += np.sum(ys*ys)

num_correct = 0
for i,(x_label,xs) in enumerate(validation_sample):
	sums = abmat[i,:]

	minimum_sum = None
	minimum_label = None
	for s,(y_label,_) in zip(sums,testing_sample):
		if minimum_sum is None or s < minimum_sum:
			minimum_sum = s
			minimum_label = y_label
	if minimum_label == x_label:
		num_correct += 1

print(num_correct * 100.0 / len(validation_sample))

elapsed = time.time() - start_time
print("elapsed time: {}".format(elapsed))
