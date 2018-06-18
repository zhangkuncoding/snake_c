struct Direct {
	int m_x;
	int m_y;
};

struct Node {
	int m_x;
	int m_y;
	struct Node *prev;
	struct Node *next;
};

void init_game();

int set_ticker(int n_msecs);

void show();

void show_information();

void show_snake();

void get_order();

void over(int error_num);

void create_dlist();

void insert_node_after_head(int x, int y);

void delete_node_before_end();

void delete_dlist();
