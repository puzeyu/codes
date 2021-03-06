#include<iostream>
#include<vector>
#include<queue>

using namespace std;

struct TreeNode {
    int val;
    int height;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x):val(x),height(1),left(NULL),right(NULL){}
};
class BinarySearchTree{
    private:
        void InOrder(TreeNode*) const;
        void PreOrder(TreeNode*) const;
        void PostOrder(TreeNode*) const;
        void LevelOrder(TreeNode*) const ;
        TreeNode* BuildHelper(const vector<int>& in,int s,int e);
        TreeNode* delHelper(TreeNode* root,const int key);
        inline int  getHeight(TreeNode* root)
        {
            return root==NULL?0:root->height;
        }
		TreeNode* RightRotation(TreeNode* node);
		TreeNode* LeftRotation(TreeNode* node);
        inline int GetBalance(TreeNode* root)
        {
            return getHeight(root->left)-getHeight(root->right);
        }

        TreeNode* root=NULL;
        const bool DEBUG=0;

    public:
		//void test()
		//{
			//root=LeftRotation(root);
		//}
        BinarySearchTree(const vector<int>);
        BinarySearchTree(void){;}
        ~BinarySearchTree();
        void InOrder(void) const
        {
            if(DEBUG) cout<<"InOrder:";
            InOrder(root);
            cout<<endl;
        }
        void PreOrder(void) const
        {
            if(DEBUG) cout<<"PreOrder:";
            PreOrder(root);
            cout<<endl;
        }
        void PostOrder(void) const
        {
            if(DEBUG) cout<<"PostOrder:";
            PostOrder(root);
            cout<<endl;
        }
        void LevelOrder(void)
        {
            if(DEBUG) cout<<"LevelOrder:\n";
            LevelOrder(root);
        }
        friend std::ostream& operator<<(std::ostream& stream,const BinarySearchTree& BST)
        {
            BST.InOrder();
            return stream;
        }

        TreeNode* searchKey(const int key) const;
        TreeNode* insert(const int key)
		{
			return root=insertHelper(root,key);
		}
        TreeNode* insertHelper(TreeNode* root,const int key);
        TreeNode* del(const int key){if(DEBUG) cout<<"del"<<key<<endl;return root=delHelper(root,key);}
        TreeNode* leftMost(TreeNode* node)
        {
            if(node==NULL) return NULL;
            while(node->left)
            {
                node=node->left;
            }
            return node;
        }

};
BinarySearchTree::BinarySearchTree(const vector<int> in )
{
    root=BuildHelper(in,0,in.size()-1);
}
BinarySearchTree::~BinarySearchTree()
{
    if(root==NULL) return ;
    queue<TreeNode*> que;
    que.push(root);
    while(!que.empty())
    {
        TreeNode* front=que.front();
        que.pop();
        if(front->left) que.push(front->left);
        if(front->right) que.push(front->right);
        if(DEBUG) { cout<<"del:"<<front->val<<endl; }
        delete front;
    }
    return ;
}
TreeNode* BinarySearchTree::BuildHelper(const vector<int>& in,int s,int e)
{
    if(s>e||s<0||e>=in.size()) return NULL;

    if(s==e)
    {
        TreeNode* node = new TreeNode(in[s]);
        if(DEBUG) cout<<"create:"<<in[s]<<endl;
        return node;
    }

    int m=(s+e)>>1;
    if(DEBUG) cout<<"create:"<<in[m]<<endl;
    TreeNode* node = new TreeNode(in[m]);
    node->left=BuildHelper(in,s,m-1);
    node->right=BuildHelper(in,m+1,e);
    return node;
}
void BinarySearchTree::InOrder(TreeNode* root) const
{
    if(root==NULL) return ;
    InOrder(root->left);
    cout<<root->val<<":"<<root->height<<" ";
    InOrder(root->right);
}
void BinarySearchTree::PreOrder(TreeNode* root) const
{
    if(root==NULL) return ;
    cout<<root->val<<":"<<root->height<<" ";
    PreOrder(root->left);
    PreOrder(root->right);
}
void BinarySearchTree::PostOrder(TreeNode* root) const
{
    if(root==NULL) return ;
    PostOrder(root->left);
    PostOrder(root->right);
    cout<<root->val<<":"<<root->height<<" ";
}
void BinarySearchTree::LevelOrder(TreeNode* root) const
{
    if(root==NULL) return ;

    queue<TreeNode*> que;
    que.push(root);
    que.push(NULL);
    while(!que.empty())
    {
        TreeNode* frt=que.front();
        que.pop();
        if(frt==NULL)
        {
            cout<<endl;
            if(!que.empty())
                que.push(NULL);
        }
        else
        {
            cout<<frt->val<<" ";
            if(frt->left) que.push(frt->left);
            if(frt->right) que.push(frt->right);
        }
    }
    return ;
}
TreeNode* BinarySearchTree:: searchKey(const int key) const
{
    TreeNode* cur=root;
    while(cur!=NULL&&cur->val!=key)
    {
        if(cur->val>key) cur=cur->left;
        else cur=cur->right;
    }
    return cur;
}
TreeNode* BinarySearchTree::insertHelper(TreeNode* root,const int key)
{
	if(root==NULL)
	{
		return new TreeNode(key);
	}
	if(root->val==key) return root;
	else if(root->val>key) {root->left=insertHelper(root->left,key);}
	else {root->right=insertHelper(root->right,key);}

	root->height=max(getHeight(root->left),getHeight(root->right))+1;
	int bal=getHeight(root->left)-getHeight(root->right);
	if(bal>1&&key<root->left->val)
		return RightRotation(root);
	else if(bal>1&&key>root->left->val)
	{
		root->left=LeftRotation(root->left);
		return RightRotation(root);
	}else if(bal<-1&&key>root->right->val)
		return LeftRotation(root);
	else if(bal<-1&&key<root->right->val)
	{
		root->right=RightRotation(root->right);
		return LeftRotation(root);
	}
	return root;
}
TreeNode* BinarySearchTree::delHelper(TreeNode* root,const int key)
{
	if(root==NULL) return NULL;
	if(root->val>key) root->left=delHelper(root->left,key);
	else if(root->val<key) root->right=delHelper(root->right,key);
    else
    {
        if(root->left==NULL)
        {
            TreeNode* right=root->right;
            delete root;
            return right;
        }else if(root->right==NULL)
        {
            TreeNode* left=root->left;
            delete root;
            return left;
        }else
        {
            TreeNode* node= leftMost(root->right);
            swap(root->val,node->val);
            root->right=delHelper(root->right,key);
            return root;
        }
    }
	root->height=max(getHeight(root->left),getHeight(root->right))+1;
    if(DEBUG) cout<<root->val<<":"<<root->height<<endl;

	int bal=getHeight(root->left)-getHeight(root->right);
	if(bal>1&&GetBalance(root->left)>=0)
		return RightRotation(root);
	else if(bal>1&&GetBalance(root->left)<0)
	{
		root->left=LeftRotation(root->left);
		return RightRotation(root);
	}else if(bal<-1&&GetBalance(root->right)<=0)
		return LeftRotation(root);
	else if(bal<-1&&GetBalance(root->right)>0)
	{
		root->right=RightRotation(root->right);
		return LeftRotation(root);
	} return root;
}
TreeNode* BinarySearchTree::RightRotation(TreeNode* node)
{
	TreeNode* left=node->left;
	TreeNode* tmp=left->right;
	left->right=node;
	node->left=tmp;
	node->height=max(getHeight(node->left),getHeight(node->right))+1;
	left->height=max(getHeight(left->right),getHeight(left->left))+1;
	return left;
}
TreeNode* BinarySearchTree::LeftRotation(TreeNode* node)
{
	TreeNode* right=node->right;
	TreeNode* tmp=right->left;
	node->right=tmp;
	right->left=node;
	node->height=max(getHeight(node->left),getHeight(node->right))+1;
	right->height=max(getHeight(right->left),getHeight(right->right))+1;
	return right;
}
int main()
{
    //vector<int> in{0,1,2,3,4,5,6,7,8,9,10};
	//BinarySearchTree BST(in);
	BinarySearchTree BST;
	for(int i=13;i>=0;i--)
	{
		BST.insert(i);
		cout<<BST;
	}
    cout<<BST;
    BST.PreOrder();
    BST.PostOrder();
    BST.LevelOrder();
    for(int i=9;i<=13;i++)
    {
        BST.del(i);
        cout<<BST;
    }
    for(int i=0;i<=8;i++)
    {
        BST.del(i);
        cout<<BST;
    }
    return 0;
}
