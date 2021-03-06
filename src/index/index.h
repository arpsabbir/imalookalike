#ifndef INDEX_H
#define INDEX_H

#include <string>
#include <vector>
#include <random>
#include <cmath>
#include <mutex>
#include <memory>

class Metric {
public:
	virtual double distance(const std::vector<double> &a, const std::vector<double> &b) = 0;
};

class Euclidean : public Metric {
public:
	double distance(const std::vector<double> &a, const std::vector<double> &b) override;
};

struct Settings {
	Metric *metric = new Euclidean();
	int M = 16;
	int M0 = 2 * M;
	int efConstruction = 100;
	int efSearch = 10;
	double mL = 1.0 / std::log(M);
	bool keepPrunedConnections = true;
};

struct SearchResult {
	std::string name;
	std::vector<double> descriptor;
	double distance;

	SearchResult(std::string name, std::vector<double> descriptor, double distance) :
		name(std::move(name)), descriptor(std::move(descriptor)), distance(distance) {}
};

class Index {
	class Node;
	struct NodeDistance;
	class NodeQueue;

	using NodePtr = std::shared_ptr<Node>;
	using NodeList = std::vector<NodePtr>;

	static std::mt19937 gen;
	static std::uniform_real_distribution<double> dist;
	static std::mutex randomMutex;

	NodePtr entryPoint = NodePtr(nullptr);
	int maxId = -1;

	std::mutex entryMutex;
	std::mutex idMutex;

	int descriptorSize;

	Metric *metric;
	int M;
	int M0;
	int efConstruction;
	int efSearch;
	double mL;
	bool keepPrunedConnections;

	static double generateRand();

	void move(Index &&other);

	double distance(const NodePtr &a, const NodePtr &b);

	NodePtr getEntryPoint();
	void setEntryPoint(const NodePtr &newEntryPoint);

	int getSize();
	int generateId();

	NodePtr createNode(std::string name, std::vector<double> descriptor, int layer);

	void searchAtLayer(const NodePtr &target, const NodePtr &entry, int searchCount, int layer,
		NodeQueue &candidates, bool *visited, int candidatesCount, NodeQueue &result);

	void selectNeighbours(const NodePtr &target, int count, int layer,
		NodeQueue &candidates, NodeList &discarded, NodeList &result);

	void load(std::string filename, Metric *metric = new Euclidean());

	NodeList collectNodes();

public:
	Index(int descriptorSize, Settings settings = Settings());

	Index(std::string dumpName, Metric *metric = new Euclidean()) {
		load(dumpName, metric);
	}

	Index(const Index&) = delete;
	Index& operator=(Index&) = delete;

	Index(Index &&other);
	Index& operator=(Index &&other);

	~Index();

	int getDescriptorSize() {
		return descriptorSize;
	}

	void insert(std::string name, std::vector<double> descriptor);
	std::vector<SearchResult> search(std::vector<double> descriptor, int k);

	void save(std::string filename);
};

class Index::Node {
public:
	std::vector<NodeList> layers;
	std::vector<std::mutex> layersMutexes;
	int maxLayer = -1;
	int id = -1;
	std::string name;
	std::vector<double> descriptor;

	Node(int id, std::string name, std::vector<double> descriptor, int layersCount, int neighboursCount, int neighboursCount0);
	Node(std::vector<double> descriptor) : descriptor(std::move(descriptor)) {}

	void addNeighbour(const NodePtr &neighbour, int layer);
};

struct Index::NodeDistance {
	double distance;
	NodePtr node;

	NodeDistance(double distance, const NodePtr &node) : distance(distance), node(node) {}
};

class Index::NodeQueue {
	class DistanceComparator {
	public:
		bool operator()(const NodeDistance &a, const NodeDistance &b) {
			return a.distance > b.distance;
		}
	};

	std::vector<NodeDistance> container;

public:
	void push(NodeDistance item);
	void emplace(double distance, const NodePtr &node);
	void popNearest();
	void popFurthest();

	const NodeDistance& nearest() {
		return container.front();
	}

	const NodeDistance& furthest() {
		return container.back();
	}

	int size() {
		return container.size();
	}

	int capacity() {
		return container.capacity();
	}

	bool empty() {
		return container.empty();
	}

	const NodeDistance& operator[](int index) {
		return container[index];
	}

	std::vector<NodeDistance>::const_iterator begin() const {
		return container.cbegin();
	}

	std::vector<NodeDistance>::const_iterator end() const {
		return container.cend();
	}

	void reserve(int size) {
		container.reserve(size);
	}

	void clear() {
		container.clear();
	}
};

#endif
