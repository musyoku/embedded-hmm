import argparse
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import model

def main():
	hmm = model.hmm(args.pool_capacity, args.sigma, args.tau, args.eta)
	outputs, sampled_states, true_states = hmm.fit(args.seq_length, args.itr)
	T = len(outputs)
	plt.figure(figsize=(16, 8))
	plt.plot(sampled_states, "black", label="new state sequence", lw=1)
	plt.scatter(np.arange(T), outputs, color="black", s=10, alpha=0.3, label="observation")
	plt.scatter(np.arange(T), true_states, color=sns.xkcd_rgb["medium green"], s=10, alpha=1.0, label="true state")

	plt.xlim(0, args.seq_length)
	plt.ylim(-10, 10)
	plt.legend(loc="upper left")
	plt.savefig("result.png")

if __name__ == "__main__":
	parser = argparse.ArgumentParser()
	parser.add_argument("-itr", "--itr", type=int, default=100)
	parser.add_argument("-seq", "--seq-length", type=int, default=1000)
	parser.add_argument("-pool", "--pool-capacity", type=int, default=20)
	
	parser.add_argument("-sigma", "--sigma", type=float, default=2.5)
	parser.add_argument("-tau", "--tau", type=float, default=0.4)
	parser.add_argument("-eta", "--eta", type=float, default=2.5)

	args = parser.parse_args()
	main()