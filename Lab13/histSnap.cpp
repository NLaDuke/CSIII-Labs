#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <algorithm>

using std::vector; using std::string;
using std::cout; using std::cin; using std::endl;

//----------------------------
// File: histSnap.cpp
// By: Nolan LaDuke
// Date: 4/21/2021
//------------------------------------------------------------------------------
// Function: File with merged + improved functionality
//            of snapshot.cpp and history.cpp
//------------------------------------------------------------------------------
// Based on a file provided by Mikhal Nesterenko, KSU
//------------------------------------------------------------------------------

// Original Document Class:
class Document {
public:
   Document(vector <string> lines={}): lines_(lines){}

   void insert(int line, const string &str) {
      int index = line-1;
      if (index <= lines_.size()+1)
	 lines_.insert(lines_.begin()+index, str);
      else
	 cout << "line out of range" << endl;
   }

   string remove(int line) {
      const int index = line-1;
      string deletedLine="";

      if(index < lines_.size()){
	       deletedLine = lines_[index];
	       lines_.erase(lines_.begin() + index);
      }
      else {cout << "line out of range" << endl;}

      return deletedLine;
   }

   void show() {
      for(int i = 0; i < lines_.size(); ++i)
	 cout << i + 1 << ". " << lines_[i] << endl;
   }

   class Memento *createMemento() const;
   void rollBack(class Memento *);

private:
   vector<string> lines_;
};

//============================== Mementos ======================================

class Memento {
public:
   Memento(const Document& doc): doc_(doc) {}
   const Document& getState() const {return doc_;}
private:
   const Document doc_;
};

Memento *Document::createMemento() const{
   // copying the Document itself
   return new Memento(*this);
}

void Document::rollBack(Memento *mem){
   *this = mem->getState();  // copying back
}

//============================= Commands =======================================

// abstract command class
class Command{
public:
   Command(Document *doc) : doc_(doc){}
   virtual void execute() = 0;
   virtual void unexecute() = 0;
   virtual string getName() = 0;
   virtual int getID() = 0;
   virtual ~Command(){}
protected:
   Document *doc_;
};

// Concrete Commands
class InsertCommand : public Command {
public:
   InsertCommand(Document *doc, int line, const string &str):
      Command(doc), line_(line),  str_(str) {}
   void execute() override { doc_->insert(line_, str_);}
   void unexecute() override {doc_->remove(line_);}
   string getName() override {return "Insert \"" + str_ + "\" to line " + std::to_string(line_);}
   int getID() override {return 0;}
private:
   int line_;
   string str_;
};


class EraseCommand : public Command {
public:
   EraseCommand(Document *doc, int line):
      Command(doc), line_(line), str_("") {}
   void execute() override {str_ = doc_->remove(line_);}
   void unexecute() override {doc_-> insert(line_, str_);}
   string getName() override {return "Erase line " + std::to_string(line_);}
   int getID() override {return 1;}
private:
   int line_;
   string str_;
};

class UndoCommand : public Command{
public:
  UndoCommand(Document *doc, Command* cmd):
      Command(doc), undoneCmd_(cmd) {}
  ~UndoCommand(){delete undoneCmd_;}
  void execute() override {undoneCmd_->unexecute();}
  void unexecute() override {undoneCmd_->execute();}
  string getName() override {return "Undo: " + undoneCmd_->getName();}
  int getID() override {return 2;}
private:
  Command* undoneCmd_;
};

class CpointCommand: public Command{
public:
  CpointCommand(Document* doc, Memento** checkpoint)
    : Command(doc), cpoint_(checkpoint){}
  void execute() override {delete *cpoint_; *cpoint_ = doc_->createMemento();};
  void unexecute() override {delete *cpoint_; *cpoint_ = nullptr;};
  string getName() override {return "Created Checkpoint";}
  int getID() override {return 3;}
private:
  Memento** cpoint_;
};

class RollbackCommand: public Command{
public:
  RollbackCommand(Document* doc, Memento** checkpoint)
      : Command(doc), cpoint_(checkpoint) {}
  void execute() override{
    if (*cpoint_ != nullptr){
     doc_->rollBack(*cpoint_);
  }}
  void unexecute() override {};
  string getName() override {return "Rollback";}
  int getID() override {return 4;}
private:
  Memento** cpoint_;
};

class RedoCommand: public Command{
public:
  RedoCommand(Document* doc, Command* cmd) : Command(doc), redoneCmd_(cmd){}
  void execute() override{redoneCmd_->execute();}
  void unexecute() override{redoneCmd_->unexecute();}
  string getName() override{return redoneCmd_->getName();}
  int getID() override {return 5;}
private:
  Command* redoneCmd_;
};


//============================ Client ==========================================

class FunctionalDocument{
public:
   FunctionalDocument(const vector<string> & text={}) : doc_(text){}

  // Function to cleanse all stored information
   void purge(){
    // Erase History-Specific
     std::for_each(history_.begin(), history_.end(),
     [](Command* cmd){
       if(cmd->getID() == 2){
         delete cmd;
       }
     });

    // Erase Rest
     std::for_each(doneCommands_.begin(), doneCommands_.end(),
     [](Command* cmd){
       delete cmd;
     });
   }

   // Destructor
   ~FunctionalDocument(){
     purge();
     if(checkpoint_ != nullptr) delete checkpoint_;
   }

// Commands:
 // Insert Line
   void insert(int line, string str) {
      Command *com = new InsertCommand(&doc_, line, str);
      com->execute();
      doneCommands_.push_back(com);
      history_.push_back(com);
   }

 // Erase line
   void erase(int line){
      Command *com = new EraseCommand(&doc_, line);
      com->execute();
      doneCommands_.push_back(com);
      history_.push_back(com);
   }

 // Undo Command
   void undo() {
      if(doneCommands_.size() != 0) {
	       Command *com = new UndoCommand(&doc_, doneCommands_.back());
	       doneCommands_.pop_back();
         com->execute();
         history_.push_back(com);
      }
      else cout << "no commands to undo" << endl;
   }

 // Create checkpoint
   void checkpoint(){
     Command* com = new CpointCommand(&doc_, &checkpoint_);
     com->execute();
     doneCommands_.push_back(com);
     history_.push_back(com);
   }

// Revert state to checkpoint
   void rollback(){
     // Purge History + Commands:
      purge();

      history_.resize(0);
      doneCommands_.resize(0);

      Command* com = new RollbackCommand(&doc_, &checkpoint_);
      com->execute();
      doneCommands_.push_back(com);
      history_.push_back(com);
   }

 // Redo previous command
   void redoCmd(int line){
     if(line < 0 || line >= history_.size()){
       cout << "There was an issue with the line number you input, please try again"
            << endl;
       return;
     }
     Command* com = new RedoCommand(&doc_, history_[line]);
     com->execute();
     doneCommands_.push_back(com);
     history_.push_back(com);
   }

 // Display history of executed commands
   void hist(){
     cout << endl << "History of commands: " << endl;
     int index = 0;
     std::for_each(history_.begin(), history_.end(),
      [&index](Command* cmd){
        cout << index++ << ". " << cmd->getName() << endl;
      });
      cout << "-------------------------------------" << endl << endl;
   }

   void show() {doc_.show();}

private:
   Document doc_;
   Memento* checkpoint_ = nullptr;
   std::deque<Command*> doneCommands_; // Does not contain undone commands
   std::deque<Command*> history_; // Contains record of undone commands
};


//================================ Main Method =================================
int main() {

   FunctionalDocument doc({
         "Lorem Ipsum is simply dummy text of the printing and typesetting",
         "industry. Lorem Ipsum has been the industry's standard dummy text",
         "ever since the 1500s, when an unknown printer took a galley of",
	       "type and scrambled it to make a type specimen book. It has",
	       "survived five centuries."});

   char option;
   do{

      doc.show();
      cout << endl;

      cout << "Enter option (i)nsert line (e)rase line (u)undo last command" << endl
           <<  "(c)heckpoint roll(b)ack (h)history (r)edo command:";
      cin >> option;

      int line; string str;
      switch(option) {

      case 'i': // Insert Line
         cout << "line number to insert: ";
         cin >> line;
         cout << "line to insert: ";
	       cin.get();
         getline(cin, str);
         doc.insert(line, str);
         break;

      case 'e': // Erase Line
         cout << "line number to remove: ";
         cin >> line;
         doc.erase(line);
         break;

      case 'c': // Checkpoint
	       doc.checkpoint();
	       break;

      case 'b': // Rollback
      	 doc.rollback();
         break;

      case 'u': // Undo
        doc.undo();
        break;

      case 'h': // History
        doc.hist();
        break;

      case 'r': // Redo
        cout << "input number to repeat: ";
        cin >> line;
        doc.redoCmd(line);
        break;

      }
   }while(option == 'i' || option == 'e' || option == 'c' || option == 'b' || option == 'u' || option == 'e' || option == 'h' || option == 'r');
}
