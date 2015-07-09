/*
**	generisk liste (c) flower/prjdd oct'1996.
**
**	implementerer objektet g_list:
**
**	g_list(int type);	# type angiver hvordan der indsaettes
**				# obj i listen.
**	reset();		# nulstiller "current" ptr til "head".
**	next();			# saetter "current" til naeste i listen.
**	prev();			# omvendt ditto :)
**	T *get();		# returnerer obj som "current" peger paa.
**	add(T *obj,float key);	# tilf0jer obj til listen med n0gle key.
**	int is_next()		# tjekker om der er et next-obj;
**	int is_prev()		# omvendt ditto.
**
**
**	hvad der mangler:
**		* INSERT_SORTED|INSERT_REVERSE ting.
**		* en reset_tail() funktion maaske ?
**
*/

#include <iostream.h>

#define INSERT_NOORDER	0
#define	INSERT_SORTED	1
#define	INSERT_REVERSE	2

// define g_list so g_list_node can find it.
template <class T> class g_list;

// template for a container-node in g_list.
template <class T> class g_list_node {
	T *obj;
	g_list_node<T> *next,*prev;
	float num_key;

	// allow g_list to access our private parts.
	friend g_list<T>;
public:
	g_list_node(T *newobj,float key);
	set_key(float k) { num_key=k; }
};

template <class T> g_list_node<T>::g_list_node(T *newobj,float key) {
	prev=next=NULL;
	num_key=key;
	obj=newobj;
}

template <class T> class g_list {
	g_list_node<T> *cur,*head,*tail;
	int insert_type;
public:
	g_list(int type);
	~g_list();
	reset();
	next();
	prev();
	int is_good();
	T *get();
	add(T *obj,float key);
};

template <class T> int g_list<T>::is_good() {
	return (cur!=NULL);
}

/*
template <class T> g_list<T>::addsorted(g_list_node<T> *o) {

}
*/

template <class T> g_list<T>::add(T *obj,float key) {
	// create the container-node.
	g_list_node<T> *t=new g_list_node<T>(obj,key);

	if (insert_type==INSERT_SORTED) {
		if (head) {
			g_list_node<T> *tmp=head;
			while (tmp->next&&(tmp->num_key>key))
				tmp=tmp->next;

			// means tmp->next == 0, so we're at last element.
			if (tmp->num_key>key) {
				tmp->next=t;
				t->prev=tmp;
			} else {
				g_list_node<T> *tt=tmp;
				tt=tmp->prev;
				tmp->prev=t;
				t->next=tmp;
				if (tt)
					tt->next=t;
				t->prev=tt;
				if (tmp==head)
					head=t;
			}
		} else
			head=tail=t;

	// insert unsorted reverse.
	} else if (insert_type==INSERT_REVERSE) {
		if (head)
			head->prev=t;
		t->next=head;
		head=t;

	// else, just put it in the list.
	} else {
		if (tail) {
			t->prev=tail;
			tail->next=t;
			tail=t;
		} else
			head=tail=t;
	}
}

template <class T> T *g_list<T>::get() {
	if (cur)
		return cur->obj;
	else
		return NULL;
}

template <class T> g_list<T>::prev() {
	if (cur)
		cur=cur->prev;
}

template <class T> g_list<T>::next() {
	if (cur)
		cur=cur->next;
}

template <class T> g_list<T>::reset() {
	cur=head;
}

template <class T> g_list<T>::g_list(int type) {
	cur=head=tail=NULL;
	insert_type=type;
}

// destructor only removes containers, not actual objects in the list.
template <class T> g_list<T>::~g_list() {
	g_list_node<T> *t;

	while (head) {
		t=head;
		head=head->next;
		delete t;
	}
}
