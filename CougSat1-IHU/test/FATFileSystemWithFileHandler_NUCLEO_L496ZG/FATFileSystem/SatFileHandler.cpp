#include "SatFileHandler.h"

SatFileHandler::~SatFileHandler(){
    
    
}

bool SatFileHandler::writef(string filenameBase, const char *message){

    std::fstream file; 
    std::string fileName = "fs/" + filenameBase;
    
    string currentString = "";
    
    for (int i = this->current; i; i/=10){

        currentString = (char)((i%10)+'0')+currentString;
        
        }
    fileName += currentString + ".txt";
    file.open(fileName, std::ios::out | std::ios::app);

    if (!file.is_open()){
        if (pc){
            pc->printf("ERROR failed to open file");
        }

        return false;
    }

    file << message << std::endl;

    file.close();

    this->current++;
    return true;
}

bool SatFileHandler::write(string filenameBase, string &message){
    return this->writef(filenameBase, message.c_str());
}

bool SatFileHandler::writeStart(){
    bool t;
    while (!this->inputMessages.empty()) {
        pair<string, string> p = this->inputMessages.front();
        t = write(get<0>(p), get<1>(p));
        this->inputMessages.pop();
    }
    return t;
}

std::ofstream &SatFileHandler::read(string &fileNameFull){
    std::ofstream *file;

    file = new std::ofstream(fileNameFull);

    if(!file->is_open()){
        
    }

    return *file;
}


bool SatFileHandler::clean(){
    DIR *d = opendir("/fs/");
    struct dirent ent;
    if (d->read(&ent)){}

    this->priority++;
    return true;
}


bool SatFileHandler::check(){
    if (this->size() < 512*10){
        if (clean()) {
            return true;
        }
    } else {
        return false;
    }
    
    return true;
}

bool SatFileHandler::enqueueMessage(pair<string, string> message){

    //Mutex mute;

    //mute.lock();
    inputMessages.push(message);
    //mute.unlock();
}
