#include <iostream>
#include <array>
#include <bits/stdc++.h>
#include <fstream>

using namespace std;


string delSpaces(string &str) 
{
   str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
   return str;
}

array<string,5> create_array_from(string line) {
    //ARRAY WHERE I WILL PUT EACH PART OF line
    array<string,5> line_parts = {"NULL","NULL","NULL","NULL","NULL"};

    int iterador = 0;
    for (int i = 0; i <line.length();i++){
        if (line[i] == ' '){
            iterador++;
            continue;
        }
        if (line_parts[iterador] == "NULL"){
            line_parts[iterador] = line[i];
        }
        else{
            line_parts[iterador] = line_parts[iterador] + line[i]; 
        }   
    }
    //cout<<iterador;
    return line_parts;
}

int main() {
    //BUS DE CONTROL
    string unidad_de_control = "";
    int acumulator_register = NULL;
    int ALU = NULL;
    
    //REGISTERS
    int pc = 0;
    string icr = "";
    int mar = NULL;
    int mdr = NULL;

    //MEMORY
    int memory_size = 1000;
    string memory_adresses[memory_size]; //[D0, D1,...D99]
    int memory_values[memory_size];//      [3,  8,....1]
    for (int i = 0; i<memory_size; i++){//map the memory adresses with it's Dn's
        memory_adresses[i] = "D" + to_string(i);
        memory_values[i] = 0;
    }

    string output = "";

    fstream newfile;
    newfile.open("instructions.txt",ios::in);
    string line;
    if (newfile.is_open())
    {
    while (getline(newfile,line)){

        array<string,5> parts_of_line = create_array_from(line);
        
        string str_first_of_line(delSpaces(parts_of_line[0]));
        string str_second_of_line(delSpaces(parts_of_line[1]));
        string str_third_of_line(delSpaces(parts_of_line[2]));
        string str_fourth_of_line(delSpaces(parts_of_line[3]));
        string str_fifth_of_line(delSpaces(parts_of_line[4]));
        string str_END("END");
        string str_ADD("ADD");
        string str_SET("SET");
        string str_SUB("SUB");
        string str_MUL("MUL");
        string str_DIV("DIV");
        string str_INC("INC");
        string str_DEC("DEC");
        string str_MOV("MOV");
        string str_LDR("LDR");
        string str_STR("STR");
        string str_BEQ("BEQ");
        string str_SHW("SHW");

        //LOGIC
        mar = pc;
        icr = line;
        pc++;

        if(str_first_of_line == str_END){
            //FINISH READING INSTRUCTION
            cout << output.substr(0,output.length()-1);
            break;
        }
        else if (str_first_of_line == str_SET){
            //MEM - SET D1 X NULL NULL, Store X value in in D1 memory address. 
            //where X is an immediate, direct or constant value. When SET instruction
            //is read the X value is stored in Memory without processor execution.
            
            int position_of_memory = stoi(str_second_of_line.substr(1,str_second_of_line.length()));

            //LOGIC
            mar = position_of_memory;
            mdr = stoi(str_third_of_line);

            //I SET THE THIRD VALUE OF THE LINE IN THE POSITION OF THE MEMORY
            memory_values[position_of_memory] = stoi(str_third_of_line);
        }
        else if (str_first_of_line == str_ADD){
            //ADD - ADDITION - There are three ways: 
            //ADD D1 NULL NULL, adds the value in D1 memory address to loaded value in accumulator register. 
            int position_of_memory = stoi(str_second_of_line.substr(1,str_second_of_line.length()));
            
            if (str_third_of_line == "NULL"){

                //LOGIC
                mar = position_of_memory;
                mdr = memory_values[position_of_memory];
                unidad_de_control = line;
                ALU = acumulator_register + memory_values[position_of_memory];

                if (acumulator_register == NULL)
                    acumulator_register = memory_values[position_of_memory];
                else{
                   acumulator_register = acumulator_register + memory_values[position_of_memory];

                }
            }
            
            //ADD D1 D3 NULL Load the value in D1 memory address in the acumulator register and adds 
            //to found value in D3 memory address. 
            else if (str_fourth_of_line == "NULL"){
                int second_position_of_memory = stoi(str_third_of_line.substr(1,str_third_of_line.length()));

                //LOGIC
                mar = position_of_memory;
                mdr = memory_values[position_of_memory];
                unidad_de_control = line;
                ALU = memory_values[position_of_memory];

                mar = second_position_of_memory;
                mdr = memory_values[second_position_of_memory];
                ALU = ALU + memory_values[second_position_of_memory];


                acumulator_register = memory_values[position_of_memory] + memory_values[second_position_of_memory];
            }
            //ADD D1 D3 D4 same that ADD D1 D3 but puts the result in D4
            else if (str_fifth_of_line == "NULL"){
                int second_position_of_memory = stoi(str_third_of_line.substr(1,str_third_of_line.length()));
                int third_position_of_memory = stoi(str_fourth_of_line.substr(1,str_fourth_of_line.length()));
                
                //LOGIC
                mar = position_of_memory;
                mdr = memory_values[position_of_memory];
                unidad_de_control = line;
                ALU = memory_values[position_of_memory];

                mar = second_position_of_memory;
                mdr = memory_values[second_position_of_memory];
                ALU = ALU + memory_values[second_position_of_memory];

                acumulator_register = memory_values[position_of_memory] + memory_values[second_position_of_memory];

                mar = third_position_of_memory;
                mdr = ALU;

                memory_values[third_position_of_memory] = acumulator_register;


            }
        }
        else if (str_first_of_line == str_SUB){
            //SUB - SUBSTRACTION - There are three ways: 
            //SUB D1 NULL NULL, substract the value in D1 memory address to loaded value in accumulator register. 
            int position_of_memory = stoi(str_second_of_line.substr(1,str_second_of_line.length()));
            if (str_third_of_line == "NULL"){

                //LOGIC
                mar = position_of_memory;
                mdr = memory_values[position_of_memory];
                unidad_de_control = line;
                ALU = acumulator_register - memory_values[position_of_memory];
                
                if (acumulator_register == NULL)
                    acumulator_register = -memory_values[position_of_memory];
                else
                    acumulator_register -= memory_values[position_of_memory];
            }
            //SUB D1 D3 NULL Load the value in D1 memory address in the acumulator register and SUBSTRACTS 
            //to found value in D3 memory address. 
            else if (str_fourth_of_line == "NULL"){
                int second_position_of_memory = stoi(str_third_of_line.substr(1,str_third_of_line.length()));

                //LOGIC
                mar = position_of_memory;
                mdr = memory_values[position_of_memory];
                unidad_de_control = line;
                ALU = memory_values[position_of_memory];

                mar = second_position_of_memory;
                mdr = memory_values[second_position_of_memory];
                ALU = ALU - memory_values[second_position_of_memory];

                acumulator_register = memory_values[position_of_memory] - memory_values[second_position_of_memory];
            }
            //SUB D1 D3 D4 same that ADD D1 D3 but puts the result in D4
            else if (str_fifth_of_line == "NULL"){
                int second_position_of_memory = stoi(str_third_of_line.substr(1,str_third_of_line.length()));
                int third_position_of_memory = stoi(str_fourth_of_line.substr(1,str_fourth_of_line.length()));

                //LOGIC
                mar = position_of_memory;
                mdr = memory_values[position_of_memory];
                unidad_de_control = line;
                ALU = memory_values[position_of_memory];

                mar = second_position_of_memory;
                mdr = memory_values[second_position_of_memory];
                ALU = ALU - memory_values[second_position_of_memory];

                acumulator_register = memory_values[position_of_memory] - memory_values[second_position_of_memory];

                mar = third_position_of_memory;
                mdr = ALU;

                memory_values[third_position_of_memory] = acumulator_register;
                
            }
        }
        else if (str_first_of_line == str_MUL){
            //MUL - MULTIPLICATION - There are three ways: 
            //MUL D1 NULL NULL, MULTIPLIES the value in D1 memory address to loaded value in accumulator register. 
            int position_of_memory = stoi(str_second_of_line.substr(1,str_second_of_line.length()));
            if (str_third_of_line == "NULL"){

                //LOGIC
                mar = position_of_memory;
                mdr = memory_values[position_of_memory];
                unidad_de_control = line;
                ALU = acumulator_register * memory_values[position_of_memory];

                if (acumulator_register == NULL)
                    acumulator_register = 0;
                else
                    acumulator_register = acumulator_register * memory_values[position_of_memory];
            }
            //MUL D1 D3 NULL Load the value in D1 memory address in the acumulator register and MULTIPLIES 
            //to found value in D3 memory address. 
            else if (str_fourth_of_line == "NULL"){
                int second_position_of_memory = stoi(str_third_of_line.substr(1,str_third_of_line.length()));

                //LOGIC
                mar = position_of_memory;
                mdr = memory_values[position_of_memory];
                unidad_de_control = line;
                ALU = memory_values[position_of_memory];

                mar = second_position_of_memory;
                mdr = memory_values[second_position_of_memory];
                ALU = ALU * memory_values[second_position_of_memory];

                acumulator_register = memory_values[position_of_memory] * memory_values[second_position_of_memory];
            }
            //MUL D1 D3 D4 same that MUL D1 D3 but puts the result in D4
            else if (str_fifth_of_line == "NULL"){
                int second_position_of_memory = stoi(str_third_of_line.substr(1,str_third_of_line.length()));
                int third_position_of_memory = stoi(str_fourth_of_line.substr(1,str_fourth_of_line.length()));

                //LOGIC
                mar = position_of_memory;
                mdr = memory_values[position_of_memory];
                unidad_de_control = line;
                ALU = memory_values[position_of_memory];

                mar = second_position_of_memory;
                mdr = memory_values[second_position_of_memory];
                ALU = ALU * memory_values[second_position_of_memory];

                acumulator_register = memory_values[position_of_memory] * memory_values[second_position_of_memory];

                mar = third_position_of_memory;
                mdr = ALU;

                memory_values[third_position_of_memory] = acumulator_register;
                
            }
        }
        else if (str_first_of_line == str_DIV){
            //DIV - DIVISION - There are three ways: 
            //DIV D1 NULL NULL, DIVIDES the value in D1 memory address to loaded value in accumulator register. 
            int position_of_memory = stoi(str_second_of_line.substr(1,str_second_of_line.length()));

            

            if (str_third_of_line == "NULL"){

                //LOGIC
                mar = position_of_memory;
                mdr = memory_values[position_of_memory];
                unidad_de_control = line;
                ALU = acumulator_register / memory_values[position_of_memory];

                if (acumulator_register == NULL)
                    acumulator_register = memory_values[position_of_memory] / 0; //tendria que dar una excepcion pero no estoy seguro si entiendo bien que pasa en este caso
                else
                    acumulator_register = memory_values[position_of_memory] / acumulator_register;
            }
            //DIV D1 D3 NULL Load the value in D1 memory address in the acumulator register and DIVIDES 
            //to found value in D3 memory address. 
            else if (str_fourth_of_line == "NULL"){
                int second_position_of_memory = stoi(str_third_of_line.substr(1,str_third_of_line.length()));

                //LOGIC
                mar = position_of_memory;
                mdr = memory_values[position_of_memory];
                unidad_de_control = line;
                ALU = memory_values[position_of_memory];

                mar = second_position_of_memory;
                mdr = memory_values[second_position_of_memory];
                ALU = ALU / memory_values[second_position_of_memory];

                acumulator_register = memory_values[position_of_memory] / memory_values[second_position_of_memory];
            }
            //DIV D1 D3 D4 same that DIV D1 D3 but puts the result in D4
            else if (str_fifth_of_line == "NULL"){
                int second_position_of_memory = stoi(str_third_of_line.substr(1,str_third_of_line.length()));
                int third_position_of_memory = stoi(str_fourth_of_line.substr(1,str_fourth_of_line.length()));

                //LOGIC
                mar = position_of_memory;
                mdr = memory_values[position_of_memory];
                unidad_de_control = line;
                ALU = memory_values[position_of_memory];

                mar = second_position_of_memory;
                mdr = memory_values[second_position_of_memory];
                ALU = ALU / memory_values[second_position_of_memory];

                acumulator_register = memory_values[position_of_memory] / memory_values[second_position_of_memory];

                mar = third_position_of_memory;
                mdr = ALU;

                memory_values[third_position_of_memory] = acumulator_register;
            }
        }
        else if (str_first_of_line == str_INC){
            //INC - INCREMENT - INC D3 NULL NULL Load the value in D3 memory address adds 1 and store in same address
            int position_of_memory = stoi(str_second_of_line.substr(1,str_second_of_line.length()));

            //LOGIC

            unidad_de_control = line;

            mar = position_of_memory;
            mdr = memory_values[position_of_memory];

            acumulator_register = memory_values[position_of_memory];//load

            ALU = memory_values[position_of_memory];
            ALU = 1 + ALU;

            acumulator_register = ALU;

            mar = position_of_memory;
            mdr = ALU; 

            memory_values[position_of_memory] = mdr;//store in same adress
        }
        else if (str_first_of_line == str_DEC){
            //DEC - DECREMENT - DEC D3 NULL NULL Load the value in D3 memory address adds 1 and store in same address
            int position_of_memory = stoi(str_second_of_line.substr(1,str_second_of_line.length()));
            //LOGIC

            unidad_de_control = line;

            mar = position_of_memory;
            mdr = memory_values[position_of_memory];

            acumulator_register = memory_values[position_of_memory];//load

            ALU = memory_values[position_of_memory];
            ALU = ALU - 1;

            acumulator_register = ALU;

            mar = position_of_memory;
            mdr = acumulator_register; 

            memory_values[position_of_memory] = mdr;//store in same adress
        }
        else if (str_first_of_line == str_MOV){
            //MOVE - MOV D2 D10 NULL Load the value in D2 memory address to D10 memory address and clear D2 address
            int position_of_memory_from = stoi(str_second_of_line.substr(1,str_second_of_line.length()));
            int position_of_memory_to = stoi(str_third_of_line.substr(1,str_third_of_line.length()));

            //LOGIC

            unidad_de_control = line;

            mar = position_of_memory_from;
            mdr = memory_values[position_of_memory_from];
            acumulator_register = mdr;

            mar = position_of_memory_to;
            mdr = acumulator_register;

            memory_values[position_of_memory_to] = memory_values[position_of_memory_from];//puts the value of the first direction in the second one
            memory_values[position_of_memory_from] = -1;
        }
        else if (str_first_of_line == str_LDR){
            //LOAD - LDR D3 NULL NULL Load the value in D3 memory address and puts in acumulator register
            int position_of_memory = stoi(str_second_of_line.substr(1,str_second_of_line.length()));

            unidad_de_control = line;

            mar = position_of_memory;
            mdr = memory_values[position_of_memory];

            acumulator_register = memory_values[position_of_memory];
        }
        else if (str_first_of_line == str_STR){
            // STORE - STR D3 NULL NULL Read the value in acumulator register and puts in D3 memory address
            int position_of_memory = stoi(str_second_of_line.substr(1,str_second_of_line.length()));

            unidad_de_control = line;

            mar = position_of_memory;
            mdr = acumulator_register;

            memory_values[position_of_memory] = acumulator_register;
        }
        else if (str_first_of_line == str_BEQ){
            //BEQ - EQUAL - BEQ D10 NULL NULL Load the value in D10 memory address if substration
            //with acumulator register values is zero puts in D10 memory address. There are three 
            //ways: BEQ D10 NULL NULL, BEQ D1 D10 NULL, BEQ D1 D2 D3

            unidad_de_control = line;

            if (str_third_of_line == "NULL"){
                int position_of_memory = stoi(str_second_of_line.substr(1,str_second_of_line.length()));

                mar = position_of_memory;
                mdr = memory_values[position_of_memory];

                ALU = mdr - acumulator_register;

                if (ALU == 0){
                    acumulator_register = ALU;
                    memory_values[position_of_memory] = acumulator_register;
                }
            }
            else if (str_fourth_of_line == "NULL"){
                int position_of_memory_one = stoi(str_second_of_line.substr(1,str_second_of_line.length()));     

                mar = position_of_memory_one;
                mdr = memory_values[position_of_memory_one];

                ALU = mdr - acumulator_register;

                if (ALU == 0){
                    acumulator_register = ALU;
                    memory_values[position_of_memory_one] = 0;
                }
                int position_of_memory_two = stoi(str_third_of_line.substr(1,str_third_of_line.length()));   

                mar = position_of_memory_two;
                mdr = memory_values[position_of_memory_two];

                ALU = mdr - acumulator_register;

                if (ALU == 0){
                    acumulator_register = ALU;
                    memory_values[position_of_memory_two] = 0;
                }
            }
            else if (str_fifth_of_line == "NULL"){
                int position_of_memory_one = stoi(str_second_of_line.substr(1,str_second_of_line.length())); 

                mar = position_of_memory_one;
                mdr = memory_values[position_of_memory_one];

                ALU = mdr - acumulator_register;

                if (ALU == 0){
                    acumulator_register = ALU;
                    memory_values[position_of_memory_one] = 0;
                }
                int position_of_memory_two = stoi(str_third_of_line.substr(1,str_third_of_line.length())); 

                mar = position_of_memory_two;
                mdr = memory_values[position_of_memory_two];

                ALU = mdr - acumulator_register;              
                if (ALU == 0){
                    acumulator_register = ALU;
                    memory_values[position_of_memory_two] = 0;
                }
                int position_of_memory_third = stoi(str_fourth_of_line.substr(1,str_fourth_of_line.length()));  
                mar = position_of_memory_third;
                mdr = memory_values[position_of_memory_third];

                ALU = mdr - acumulator_register;               
                if (ALU == 0){
                    acumulator_register = ALU;
                    memory_values[position_of_memory_third] = 0;
                }
            }
        }
        else if(str_first_of_line == str_SHW){
            unidad_de_control = line;
            if (str_second_of_line == "ACC"){
                string str_acc = to_string(acumulator_register);
                output = output + str_acc.append("\n");
                //cout << acumulator_register << endl;
            }
            else if (str_second_of_line == "ICR"){
                string str_icr = icr;
                output = output + str_icr.append("\n");
                //cout << icr << endl;
            }
            else if (str_second_of_line == "MAR"){
                string str_mar = to_string(mar);
                output = output + str_mar.append("\n");
                //cout << mar << endl;
            }
            else if (str_second_of_line == "MDR"){
                string str_mdr = to_string(mdr);
                output = output + str_mdr.append("\n");
                //cout << mdr << endl;
            }
            else if(str_second_of_line == "UC"){
                string str_uc = unidad_de_control;
                output = output + str_uc.append("\n");
                //cout << unidad_de_control << endl;
            }
            else{
                int position_of_memory = stoi(str_second_of_line.substr(1,str_second_of_line.length()));

                mar = position_of_memory;
                mdr = memory_values[position_of_memory];

                string str_mdr = to_string(memory_values[position_of_memory]);
                output = output + str_mdr.append("\n");


                //cout << memory_values[position_of_memory] << endl;
            }
        }
        else{//for debugging, if it didn't get throw the upper ones,
             //it's maybe because it has a space at the end
             //it only happens if the there are more than two words in line
             //to solve that, when getting each part of the line use delSpaces function
            cout << "ERROR BEGIN" << endl;
            cout<<"WW" + str_first_of_line + "WW"<< endl;
            cout << "ERROR END";
        }
        
    }
    newfile.close();
    }
    
    
    return 0;
}

