struct Node {
	bool isleaf;
	Node *children[8];
	void *data;
};

class SparseVoxelOctree {
	private:

	Node *root;

	public:

	bool insert(char pos, float size) {

	}

	SparseVoxelOctree() {
		root = new Node;
	}
	~SparseVoxelOctree() {
		delete root;
	}
};
