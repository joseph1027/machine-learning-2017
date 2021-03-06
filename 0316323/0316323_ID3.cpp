#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<math.h>
#include<algorithm>
#include<time.h>

using namespace std;
#define number_of_data 150
#define number_of_attribute 4
#define number_of_category 3

class flower
{
public:
    float sepal_length;
    float sepal_width;
    float petal_length;
    float petal_width;
    string flower_name;
};

class node
{
    public:
        vector<flower> node_data_list;
        node* left_child;
        node* right_child;
        bool leaf;
        string category;
        float node_point;
        int node_column;
    node()
    {
        left_child=NULL;
        right_child=NULL;
        leaf = false;
    }
};

vector <flower> flower_table;

class tree
{
    public:
    node* root;

    tree()
    {
        srand(time(0));
        root = NULL;
        read_txt();
        tree_train();
    }

    void read_txt()
    {
      ifstream f_ptr;
  		f_ptr.open("data.txt");

  		flower in_flower;
  		string tmp_string;
  		for(int i=0;i<number_of_data;i++)
  		{
  	    getline(f_ptr,tmp_string);
  			in_flower.sepal_length=stof(tmp_string.substr(0,3));
  			in_flower.sepal_width=stof(tmp_string.substr(4,3));
  			in_flower.petal_length=stof(tmp_string.substr(8,3));
  			in_flower.petal_width=stof(tmp_string.substr(12,3));
  			in_flower.flower_name=tmp_string.substr(16,tmp_string.size()-16+1);
  			flower_table.push_back(in_flower);
  		}
    }

    void delete_tree(struct node* now_node)
  	{
    		 if(now_node!=NULL)
   		   {
      			delete_tree(now_node->left_child);
            delete_tree(now_node->right_child);
      			free(now_node);
    		 }
  	}

    void make_random()
    {
        time_t xx;
        srand((unsigned)time(NULL));
        for(int i=0;i<=9987;i++)
        {
            int m = rand()%number_of_data;
            int n = rand()%number_of_data;
            flower tmp = flower_table[m];
            flower_table[m]=flower_table[n];
            flower_table[n]=tmp;
        }
    }

    float precision[3] = {0,0,0};
    float recall[3] = {0,0,0};

    float test(vector<flower> test_data)
    {
        string guess_category;

        int guess_counter[3]={0,0,0};
        int correct_counter[3]={0,0,0};
        int actual_counter[3]={0,0,0};
        int tp = 0;

        for(int i=0;i<test_data.size();i++)
        {

            guess_category = gogogo(root,test_data[i]);

            if(guess_category == "Iris-setosa")
            {
                guess_counter[0]++;
            }
            else if(guess_category == "Iris-versicolor")
            {
                guess_counter[1]++;
            }
            else if(guess_category == "Iris-virginica")
            {
                guess_counter[2]++;
            }


            if(test_data[i].flower_name == "Iris-setosa")
            {
                actual_counter[0]++;
            }
            else if(test_data[i].flower_name == "Iris-versicolor")
            {
                actual_counter[1]++;
            }
            else if(test_data[i].flower_name == "Iris-virginica")
            {
                actual_counter[2]++;
            }


            if(guess_category == test_data[i].flower_name)
            {
                if(guess_category == "Iris-setosa")
                {
                    tp++;
                    correct_counter[0]++;
                    //counter[0]++;
                }
                else if(guess_category == "Iris-versicolor")
                {
                    tp++;
                    correct_counter[1]++;
                    //counter[1]++;
                }
                else if(guess_category == "Iris-virginica")
                {
                    tp++;
                    correct_counter[2]++;
                    //counter[2]++;
                }
            }
        }
        //cout<<"rate:"<<(float)countt/(float)test_data.size();

        precision[0]+=correct_counter[0]/(float)guess_counter[0];
        precision[1]+=correct_counter[1]/(float)guess_counter[1];
        precision[2]+=correct_counter[2]/(float)guess_counter[2];
        recall[0]+=correct_counter[0]/(float)actual_counter[0];
        recall[1]+=correct_counter[1]/(float)actual_counter[1];
        recall[2]+=correct_counter[2]/(float)actual_counter[2];
        float accuracy = tp/(float)test_data.size();
        //cout<<"precision:"<<precision<<endl;
        //cout<<"recall:"<<recall<<endl;
        //cout<<"accuracy:"<<accuracy<<endl;
        return accuracy;
    }

    string gogogo(node* cur_node, flower ff)
    {
        string result;
        while(cur_node->leaf==false)
        {
            //cout<<"ddd"<<endl;
            if(cur_node->node_column==0)
            {
                if(ff.sepal_length<cur_node->node_point)
                {
                    cur_node = cur_node->left_child;
                }
                else
                {
                    cur_node = cur_node->right_child;
                }
            }
            else if(cur_node->node_column==1)
            {
                if(ff.sepal_width<cur_node->node_point)
                {
                    cur_node = cur_node->right_child;
                }
                else
                {
                    cur_node = cur_node->right_child;
                }
            }
            else if(cur_node->node_column==2)
            {
                if(ff.petal_length<cur_node->node_point)
                {
                    cur_node = cur_node->left_child;
                }
                else
                {
                    cur_node = cur_node->right_child;
                }
            }
            else if(cur_node->node_column==3)
            {
                if(ff.petal_width<cur_node->node_point)
                {
                    cur_node = cur_node->left_child;
                }
                else
                {
                    cur_node = cur_node->right_child;
                }
            }
        }
        return cur_node->category;

    }

    void tree_train()
    {
        //cout<<"TREE_TRAIN"<<endl;
        float accuracy1,accuracy2,accuracy3,accuracy4,accuracy5;
        root = new node;
        //cout<<"TREE_TRAIN"<<endl;
        make_random();
        //cout<<"TREE_TRAIN"<<endl;
        /////////////////////////////////////////////////////////////////////////////////
        //cout<<"Flower sizxe  "<<flower_table.size()<<endl;
        vector<flower> training_data1(flower_table.begin() , flower_table.begin()+(number_of_data*4/5));
        //cout<<"TREE_TRAIN"<<endl;
        vector<flower> test_data1(flower_table.begin()+(number_of_data*4/5),flower_table.begin() + number_of_data);
        //cout<<"TREE_TRAIN"<<endl;
        root -> node_data_list = training_data1;
        //cout<<"TREE_TRAIN"<<endl;
        build_decision_tree(root);
        //c//out<<"TREE_TRAIN"<<endl;
        //traverse(root);
        accuracy1 = test(test_data1);
        //cout<<"TREE_TRAIN"<<endl;
        delete_tree(root);
        //cout<<"TREE_TRAIN"<<endl;
        /////////////////////////////////////////////////////////////////////////////////

        root = new node;
        vector<flower> training_data2;
        for(int i=0;i<(number_of_data*3/5);i++)
        {
            training_data2.push_back(flower_table[i]);
        }
        for(int i=number_of_data*4/5;i<number_of_data;i++)
        {
            training_data2.push_back(flower_table[i]);
        }
        vector<flower> test_data2(flower_table.begin()+(number_of_data*3/5),flower_table.begin()+number_of_data*4/5);
        root -> node_data_list = training_data2;
        build_decision_tree(root);
        //traverse(root);
        accuracy2 = test(test_data2);
        delete_tree(root);
        /////////////////////////////////////////////////////////////////////////////////

        root = new node;
        vector<flower> training_data3;
        for(int i=0;i<(number_of_data*2/5);i++)
        {
            training_data3.push_back(flower_table[i]);
        }
        for(int i=number_of_data*3/5;i<number_of_data;i++)
        {
            training_data3.push_back(flower_table[i]);
        }
        vector<flower> test_data3(flower_table.begin()+(number_of_data*2/5),flower_table.begin()+number_of_data*3/5);
        root -> node_data_list = training_data3;
        build_decision_tree(root);
        //traverse(root);
        accuracy3 = test(test_data3);
        delete_tree(root);
         /////////////////////////////////////////////////////////////////////////////////

         root = new node;
        vector<flower> training_data4;
        for(int i=0;i<(number_of_data*1/5);i++)
        {
            training_data4.push_back(flower_table[i]);
        }
        for(int i=number_of_data*2/5;i<number_of_data;i++)
        {
            training_data4.push_back(flower_table[i]);
        }
        vector<flower> test_data4(flower_table.begin()+(number_of_data*1/5),flower_table.begin()+number_of_data*2/5);
        root -> node_data_list = training_data4;
        build_decision_tree(root);
        //traverse(root);
        accuracy4 = test(test_data4);
        delete_tree(root);

        root = new node;
        vector<flower> training_data5;
        for(int i=number_of_data*1/5;i<number_of_data;i++)
        {
            training_data5.push_back(flower_table[i]);
        }
        vector<flower> test_data5(flower_table.begin(),flower_table.begin()+number_of_data*1/5);
        root -> node_data_list = training_data5;
        build_decision_tree(root);
        //traverse(root);
        accuracy5 = test(test_data5);
        delete_tree(root);

        float total_accuracy = (accuracy1+accuracy2+accuracy3+accuracy4+accuracy5)/5;
        cout<<total_accuracy<<endl;

        cout<<precision[0]/5<<" "<<recall[0]/5<<endl;
        cout<<precision[1]/5<<" "<<recall[1]/5<<endl;
        cout<<precision[2]/5<<" "<<recall[2]/5<<endl;
        //cout<<root->node_data_list.size()<<endl;//120
        //cout<<test_data1.size()<<endl;//30
        /*for(int i=0; i<root->node_data_list.size();i++)
        {
            cout << root->node_data_list[i].flower_name;
        }*/

    }
    //int countt=0;
    void build_decision_tree(node* cur_node)
    {
        ////cout<<"IN BUILD_TREE"<<endl;
        vector<flower> cur_data = cur_node->node_data_list;
        //countt++;
        //cout<<"C:"<<countt<<endl;
        if(same_label(cur_data))
        {
            //cout<<"SAME"<<endl;
            cur_node->leaf = true;
            cur_node->left_child = NULL;
            cur_node->right_child = NULL;
            cur_node -> category = cur_data[0].flower_name;
            return;
        }
        else if(cur_node->leaf == false)
        {
            //cout<<"NOT LEAF"<<endl;
            float max_cut_point = 0;
            float min_entrophy = 10000;
            float tmp_entrophy = 0;
            float tmp_cut_point = 0;
            int split_column = 0;
            flower tmp;
            for(int i=0 ; i<number_of_attribute; i++)
            {
                if(i==0)
                {
                    for(int j=0; j<cur_data.size()-1;j++)
                    {
                        for(int k=0; k<cur_data.size()-j-1;k++)
                        {
                            if(cur_data[k].sepal_length>cur_data[k+1].sepal_length)
                            {
                                tmp=cur_data[k];
                                cur_data[k]=cur_data[k+1];
                                cur_data[k+1]=tmp;
                            }
                        }
                    }
                    /*cout<<"sepal_length:"<<endl;
                    for(int m=0;m<cur_data.size();m++)
                    {
                        cout<<cur_data[m].sepal_length<<endl;
                    }*/
                }
                else if(i==1)
                {
                    for(int j=0; j<cur_data.size()-1;j++)
                    {
                        for(int k=0; k<cur_data.size()-j-1;k++)
                        {
                            if(cur_data[k].sepal_width>cur_data[k+1].sepal_width)
                            {
                                tmp=cur_data[k];
                                cur_data[k]=cur_data[k+1];
                                cur_data[k+1]=tmp;
                            }
                        }
                    }
                    /*cout<<"sepal_width:"<<endl;
                    for(int m=0;m<cur_data.size();m++)
                    {
                        cout<<cur_data[m].sepal_width<<endl;
                    }*/
                }
                else if(i==2)
                {
                    for(int j=0; j<cur_data.size()-1;j++)
                    {
                        for(int k=0; k<cur_data.size()-j-1;k++)
                        {
                            if(cur_data[k].petal_length>cur_data[k+1].petal_length)
                            {
                                tmp=cur_data[k];
                                cur_data[k]=cur_data[k+1];
                                cur_data[k+1]=tmp;
                            }
                        }
                    }
                    /*cout<<"petal_length:"<<endl;
                    for(int m=0;m<cur_data.size();m++)
                    {
                        cout<<cur_data[m].petal_length<<endl;
                    }*/
                }
                else if(i==3)
                {
                    for(int j=0; j<cur_data.size()-1;j++)
                    {
                        for(int k=0; k<cur_data.size()-j-1;k++)
                        {
                            if(cur_data[k].petal_width>cur_data[k+1].petal_width)
                            {
                                tmp=cur_data[k];
                                cur_data[k]=cur_data[k+1];
                                cur_data[k+1]=tmp;
                            }
                        }
                    }
                    /*cout<<"petal_width:"<<endl;
                    for(int m=0;m<cur_data.size();m++)
                    {
                        cout<<cur_data[m].petal_width<<endl;
                    }*/
                }

                for(int row = 0 ; row<cur_data.size()-1 ; row++)
                {
                    if(cur_data[row].flower_name!=cur_data[row+1].flower_name)
                    {
                        if(i==0)
                        {
                            tmp_cut_point = (cur_data[row].sepal_length + cur_data[row+1].sepal_length)/2;
                        }
                        else if(i==1)
                        {
                            tmp_cut_point = (cur_data[row].sepal_width + cur_data[row+1].sepal_width)/2;
                        }
                        else if(i==2)
                        {
                            tmp_cut_point = (cur_data[row].petal_length + cur_data[row+1].petal_length)/2;
                        }
                        else if(i==3)
                        {
                            tmp_cut_point = (cur_data[row].petal_width + cur_data[row+1].petal_width)/2;
                        }
                        tmp_entrophy = compute_entrophy(i,cur_data,tmp_cut_point);
                        //cout<<"entrophy:"<<tmp_entrophy<<endl;
                        if(tmp_entrophy<min_entrophy)
                        {
                            min_entrophy = tmp_entrophy;
                            max_cut_point = tmp_cut_point;
                            split_column = i;
                        }

                    }

                }
            }
            cur_node->node_column = split_column;
            cur_node->node_point = max_cut_point;

            node* left_sub= new node;
            node* right_sub= new node;
            left_sub->leaf = false;
            right_sub->leaf= false;

            left_sub->node_data_list = divide_load_data(cur_data,0,split_column,max_cut_point);
            right_sub->node_data_list = divide_load_data(cur_data,1,split_column,max_cut_point);
            if(left_sub->node_data_list.size()<2||right_sub->node_data_list.size()<2)
            {
                cur_node -> left_child = NULL;
                cur_node -> right_child = NULL;
                cur_node -> leaf = true;
                cur_node -> category = majority(cur_data);
                return;
            }
            else
            {
                cur_node->left_child = left_sub;
                cur_node->right_child = right_sub;

                //cout<<split_column<<endl;
                //cout<<max_cut_point<<endl;
                //cout<<"L: "<<left_sub->node_data_list.size()<<endl;
                //cout<<"R: "<<right_sub->node_data_list.size()<<endl;
                //cout<<"LEFT!!!!!!!!!!!!!"<<endl;

                /*for(int k=0;k<left_sub->node_data_list.size();k++)
                {
                    cout<<k<<"::"<<left_sub->node_data_list[k].flower_name<<endl;
                }*/
                //cout<<"RIGHT!!!!!!!!!!!!!"<<endl;
                /*for(int k=0;k<right_sub->node_data_list.size();k++)
                {
                    cout<<k<<"::"<<right_sub->node_data_list[k].flower_name<<endl;
                }*/
                build_decision_tree(left_sub);
                /*if(left_sub->leaf==true)
                {
                    cout<<"!!!!!!!!!!"<<left_sub->category<<endl;
                }*/
                build_decision_tree(right_sub);
            }
        }
    }

    string majority(vector<flower> data)
    {
        int a=0,b=0,c=0;
        for(int i=0;i<data.size();i++)
        {
            if(data[i].flower_name == "Iris-setosa")
            {
                a++;
            }
            if(data[i].flower_name == "Iris-versicolor")
            {
                b++;
            }
            if(data[i].flower_name == "Iris-virginica")
            {
                c++;
            }
        }
        if(a>=b && a>=c)
        {
            return "Iris-setosa";
        }
        else if(b>=a && b>=c)
        {
            return "Iris-versicolor";
        }
        else if(c>=a && c>= b)
        {
            return "Iris-virginica";
        }

    }

    float compute_entrophy(int column_id, vector<flower> cur_data, float cut_point)
    {
        //cout<<"COMPUTER EMTROPHY"<<endl;
        float bigger = 0;
        float smaller = 0;
        float entrophy = 0;
        float count_flower_smaller[number_of_category]={0,0,0};
        float count_flower_bigger[number_of_category]={0,0,0};
        if(column_id == 0)
        {
            for(int i=0 ; i<cur_data.size();i++)
            {
                if(cur_data[i].sepal_length<=cut_point)
                {
                    smaller++;
                    if(cur_data[i].flower_name=="Iris-setosa")
                    {
                        count_flower_smaller[0]++;
                    }
                    else if(cur_data[i].flower_name=="Iris-versicolor")
                    {
                        count_flower_smaller[1]++;
                    }
                    else if(cur_data[i].flower_name=="Iris-virginica")
                    {
                        count_flower_smaller[2]++;
                    }
                }
                else
                {
                    bigger++;
                    if(cur_data[i].flower_name=="Iris-setosa")
                    {
                        count_flower_bigger[0]++;
                    }
                    else if(cur_data[i].flower_name=="Iris-versicolor")
                    {
                        count_flower_bigger[1]++;
                    }
                    else if(cur_data[i].flower_name=="Iris-virginica")
                    {
                        count_flower_bigger[2]++;
                    }
                }
            }
        }
        else if(column_id == 1)
        {
            for(int i=0 ; i<cur_data.size();i++)
            {
                if(cur_data[i].sepal_width<=cut_point)
                {
                    smaller++;
                    if(cur_data[i].flower_name=="Iris-setosa")
                    {
                        count_flower_smaller[0]++;
                    }
                    else if(cur_data[i].flower_name=="Iris-versicolor")
                    {
                        count_flower_smaller[1]++;
                    }
                    else if(cur_data[i].flower_name=="Iris-virginica")
                    {
                        count_flower_smaller[2]++;
                    }
                }
                else
                {
                    bigger++;
                    if(cur_data[i].flower_name=="Iris-setosa")
                    {
                        count_flower_bigger[0]++;
                    }
                    else if(cur_data[i].flower_name=="Iris-versicolor")
                    {
                        count_flower_bigger[1]++;
                    }
                    else if(cur_data[i].flower_name=="Iris-virginica")
                    {
                        count_flower_bigger[2]++;
                    }
                }
            }
        }
        else if(column_id == 2)
        {
            for(int i=0 ; i<cur_data.size();i++)
            {
                if(cur_data[i].petal_length<=cut_point)
                {
                    smaller++;
                    if(cur_data[i].flower_name=="Iris-setosa")
                    {
                        count_flower_smaller[0]++;
                    }
                    else if(cur_data[i].flower_name=="Iris-versicolor")
                    {
                        count_flower_smaller[1]++;
                    }
                    else if(cur_data[i].flower_name=="Iris-virginica")
                    {
                        count_flower_smaller[2]++;
                    }
                }
                else
                {
                    bigger++;
                    if(cur_data[i].flower_name=="Iris-setosa")
                    {
                        count_flower_bigger[0]++;
                    }
                    else if(cur_data[i].flower_name=="Iris-versicolor")
                    {
                        count_flower_bigger[1]++;
                    }
                    else if(cur_data[i].flower_name=="Iris-virginica")
                    {
                        count_flower_bigger[2]++;
                    }
                }
            }
        }
        else if(column_id == 3)
        {
            for(int i=0 ; i<cur_data.size();i++)
            {
                if(cur_data[i].petal_width<=cut_point)
                {
                    smaller++;
                    if(cur_data[i].flower_name=="Iris-setosa")
                    {
                        count_flower_smaller[0]++;
                    }
                    else if(cur_data[i].flower_name=="Iris-versicolor")
                    {
                        count_flower_smaller[1]++;
                    }
                    else if(cur_data[i].flower_name=="Iris-virginica")
                    {
                        count_flower_smaller[2]++;
                    }
                }
                else
                {
                    bigger++;
                    if(cur_data[i].flower_name=="Iris-setosa")
                    {
                        count_flower_bigger[0]++;
                    }
                    else if(cur_data[i].flower_name=="Iris-versicolor")
                    {
                        count_flower_bigger[1]++;
                    }
                    else if(cur_data[i].flower_name=="Iris-virginica")
                    {
                        count_flower_bigger[2]++;
                    }
                }
            }
        }

        if(smaller==0 || bigger==0)
        {
            return 100000;
        }
        //smaller
        for(int i=0;i<number_of_category;i++)
        {
            if(count_flower_smaller[i]!=0)
            {
                entrophy -= ((smaller/(smaller+bigger))*(count_flower_smaller[i]/smaller)*(log2(count_flower_smaller[i]/smaller)));
            }
            if(count_flower_bigger[i]!=0)
            {
                entrophy -= ((bigger/(smaller+bigger))*(count_flower_bigger[i]/bigger)*(log2(count_flower_bigger[i]/bigger)));
            }
        }
        //cout<<"RETURN ENTROPHY "<<entrophy<<endl;
        return entrophy;

    }

    vector<flower> divide_load_data(vector<flower> cur_data, int flagg, int split_column , float max_cut_point )
    {
        //cout<<"DIVIDE"<<endl;
        vector <flower> final_data;
        if(flagg==0)
        {
            if(split_column == 0)
            {
                for(int i=0 ; i<cur_data.size();i++)
                {
                    if(cur_data[i].sepal_length<max_cut_point)
                    {
                        final_data.push_back(cur_data[i]);
                    }
                }
            }
            if(split_column == 1)
            {
                for(int i=0 ; i<cur_data.size();i++)
                {
                    if(cur_data[i].sepal_width<max_cut_point)
                    {
                        final_data.push_back(cur_data[i]);
                    }
                }
            }
            if(split_column == 2)
            {
                for(int i=0 ; i<cur_data.size();i++)
                {
                    if(cur_data[i].petal_length<max_cut_point)
                    {
                        final_data.push_back(cur_data[i]);
                    }
                }
            }
            if(split_column == 3)
            {
                for(int i=0 ; i<cur_data.size();i++)
                {
                    if(cur_data[i].petal_width<max_cut_point)
                    {
                        final_data.push_back(cur_data[i]);
                    }
                }
            }
        }
        else if(flagg==1)
        {
            if(split_column == 0)
            {
                for(int i=0 ; i<cur_data.size();i++)
                {
                    if(cur_data[i].sepal_length>=max_cut_point)
                    {
                        final_data.push_back(cur_data[i]);
                    }
                }
            }
            if(split_column == 1)
            {
                for(int i=0 ; i<cur_data.size();i++)
                {
                    if(cur_data[i].sepal_width>=max_cut_point)
                    {
                        final_data.push_back(cur_data[i]);
                    }
                }
            }
            if(split_column == 2)
            {
                for(int i=0 ; i<cur_data.size();i++)
                {
                    if(cur_data[i].petal_length>=max_cut_point)
                    {
                        final_data.push_back(cur_data[i]);
                    }
                }
            }
            if(split_column == 3)
            {
                for(int i=0;i<cur_data.size();i++)
                {
                    if(cur_data[i].petal_width>=max_cut_point)
                    {
                        final_data.push_back(cur_data[i]);
                    }
                }
            }
        }
        return final_data;
    }


    bool same_label(vector<flower> data_table)
    {
        //cout<<"LABEL"<<endl;
        if(data_table.size()==1)
        {
            return true;
        }
        else
        {
            for(int i=0; i<data_table.size()-1;i++)
            {
                if(data_table[i].flower_name!=data_table[i+1].flower_name)
                {
                    return false;
                }

            }
            /*for(int i = 0 ;i<100;i++)
            {
                cout<<"HOMO"<<endl;
            }*/
            return true;
        }
    }

    void traverse(node* current_node)
    {
        if(current_node->left_child!=NULL)
        {
            traverse(current_node->left_child);
        }
        if(current_node->right_child!=NULL)
        {
            traverse(current_node->right_child);
        }
        if(current_node->leaf==true)
        {
            cout<<current_node->category<<endl;
        }
    }



};



int main()
{
    tree decision_tree;

}
