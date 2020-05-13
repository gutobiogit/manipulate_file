#include <iostream> // I/O 
#include <regex>    // Regular Expression
#include<fstream>   // File Operation
#include <math.h>   // Math
#include <iomanip>  // Formating, precision

using namespace std;

string result_data[100][100]; //
string result_data_names_attr[100][100]; //set attr variable
string result_data_names_sub[100][100];  //set sub variable
string result_operations[100][5];  //Temporarily store variable 
string result_output[100][2];  //Temporarily store variable


//work with data.xml file
void process_data()
{   
    //File
    ifstream file_data;
    file_data.open ("data.xml");
    //Position bidimensiona var
    int counter_1 = 0;
    int counter_2 = 0;
    //Name and file data  
    string name_reg,word;
    //attrib or sub
    string attrib= "attrib";
    string area;
    //flag end position
    bool end_name=false;
    //regex
    smatch result_type,result_type_name,result_type_clean;
    regex r("([a-z A-z]*?)=");
    //read the file space is the break
    while (file_data >> word)
    {   
        //regex all when attrib
        regex_search(word,result_type_name,r);
        if ((result_type_name[0]!="") && (attrib == "attrib"))
        {   
            if ((name_reg != result_type_name[1]) && (! end_name)) 
            {                  
                name_reg=result_type_name[1];
                result_data_names_attr[0][counter_2] = result_type_name[1];
            } 
            if (! word.empty())
            {   
                regex r("(.*?$)");
                regex_search(word,result_type,r);
                regex_search (word,result_type_clean,regex("\"([^\"]*)\""));
                string res_type,res_type_con;
                res_type=result_type[1];
                res_type_con=res_type.back();
                if (res_type_con == "n")
                {
                    file_data >> word;
                    res_type+=" "+word;
                    regex_search (res_type,result_type_clean,regex("\"([^\"]*)\""));
                    result_data[counter_1][counter_2] = result_type_clean[1];
                }
                else
                {
                    result_data[counter_1][counter_2] = result_type_clean[1];
                }
            }
            counter_2++;        
        }   
            regex sub_regex("<(.*?)>(.*?)</(.*?)>");
            regex_search(word,result_type,sub_regex);
        //regex all when sub   
        if ((result_type[0]!="") && (attrib == "sub"))
        {
            for (int i=0;result_data_names_attr[0][i]!="";i++)
            {
              counter_2=i;
            }
            counter_2++;
            result_data_names_sub[0][counter_2+1] = result_type[1];
            result_data[counter_1-1][counter_2]=result_type[2];
            counter_2=0;
        }
        //regex the end of attr begin sub
        regex ending_name("\">");
        regex_search(word,result_type,ending_name);
        if (! result_type.empty()) 
        {
            end_name=true;
            attrib= "sub";
            counter_1++;
            counter_2=0;
        }
        else
        {
            attrib="attrib";
        }
    }
}

//work operations.xml file
void process_operations()
{
    ifstream file_operation;
    file_operation.open ("operations.xml");
    int counter_1 = 0;
    int counter_2=0;
    string word;
    //regex the fixed operation
    while (file_operation >> word)
    {   
        //name
        smatch search_word;
        regex re_name("name=\"(.*?)\"");
        regex_search(word,search_word,re_name);
        if (search_word[1] !="")
        {
            result_operations[counter_1][counter_2] = search_word[1];
            counter_2++;
        }
        //type
        regex re_type("type=\"(.*?)\"");
        regex_search(word,search_word,re_type);
        if (search_word[1] !="")
        {
            result_operations[counter_1][counter_2] = search_word[1];
            counter_2++;
        }
        //func
        regex re_func("func=\"(.*?)\"");
        regex_search(word,search_word,re_func);
        if (search_word[1] !="")
        {
            result_operations[counter_1][counter_2] = search_word[1];
            counter_2++;
        }
        //attrib
        regex re_attr("attrib=\"(.*?)\"");
        regex_search(word,search_word,re_attr);
        if (search_word[1] !="")
        {
            result_operations[counter_1][counter_2] = search_word[1];
            counter_2++;
        }
        //filter
        regex re_filt("filter=\"(.*?)\"");
        regex_search(word,search_word,re_filt);
         if (search_word[1] !="")
        {
            result_operations[counter_1][counter_2] = search_word[1];
            counter_1++;
            counter_2=0;
        }    
    }
    file_operation.close();
}

//output operation calculus
void process_output()
{   
    float calc_result;
    int  counter_2,media_calc=0;
    //loop interaction max 100 set in global var
    for (int i = 0; i < 100; i++)
    {
        calc_result=0;
        if (result_operations[i][0] != "")
        {   
            result_output[i][0] = result_operations[i][0];
            regex re_city(result_operations[i][4]);
            for (int j = 0; j < 100; j++)
            {
                //Set the attrib request
                if (regex_match(result_data[j][0],re_city))
                {     counter_2=2;    
                        if (result_operations[i][1]=="attrib")
                        {
                            for (int k=0; result_data[0][k] !=""; k++)
                            {   
                                if (result_data_names_attr[0][k]==result_operations[i][3])
                                counter_2=k;
                            }
                        }
                        //Set the sub request
                        else if (result_operations[i][1]=="sub")
                        {   
                            for (int k=0; result_data[0][k] !=""; k++)
                            {   
                                if (result_data_names_sub[0][k]==result_operations[i][3])
                                counter_2=k;
                            }
                        }
                        //average calculation
                        if(result_operations[i][2] == "average")
                        {   
                            media_calc++;
                            calc_result=calc_result+ stof(result_data[j][counter_2]);
                        }
                        //sum calculation
                        else if (result_operations[i][2] == "sum")
                        { 
                            calc_result=calc_result + stof(result_data[j][counter_2]);
                        }
                        //min calculation
                        else if (result_operations[i][2] == "min")
                        {   
                            if (stof(result_data[j][counter_2]) < calc_result || calc_result==0)
                            {  
                                calc_result=stof(result_data[j][counter_2]);
                            }
                        }
                        //max calculation
                        else if (result_operations[i][2] == "max")
                        {
                            if (stof(result_data[j][counter_2]) > calc_result || calc_result==0)
                            {  
                                calc_result=stof(result_data[j][counter_2]);
                            }
                        }   
                }
            }
        }
        //Feed the average
        if(result_operations[i][2] == "average")
        {   
            ostringstream resu;
            resu << fixed;
            resu << setprecision(2);
            resu << (double)calc_result/media_calc ;
            result_output[i][1]=resu.str();
            calc_result=0;
        }
        //Feed the sum
        else if (result_operations[i][2] == "sum")
        {
            ostringstream resu;
            resu << fixed;
            resu << setprecision(2);
            resu << (double)calc_result;
            result_output[i][1]=resu.str();
            calc_result=0;
        }
        //Feed  the min
        else if (result_operations[i][2] == "min")
        {
            ostringstream resu;
            resu << fixed;
            resu << setprecision(2);
            resu << (double)calc_result;
            result_output[i][1]=resu.str();
            calc_result=0;
        }
        //Feed  the max
        else if (result_operations[i][2] == "max")
        {
            ostringstream resu;
            resu << fixed;
            resu << setprecision(2);
            resu << (double)calc_result;
            result_output[i][1]=resu.str();
            calc_result=0;
        }   
    }
}

int main()
{   
    process_data();
    process_operations();
    process_output();
    //Save file output.xml
    ofstream out("output.xml");
    out << "<results>" << endl;
    for (int i=0;! result_output[i][0].empty(); i++)
        {
            out << "   <result name=" << "\"" <<result_output [i][0] << "\"" << ">" << result_output [i][1] << "</result>" << endl;
        }
    out << "</results>";
    out.close();
    return 0;
}