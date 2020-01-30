#include "SatFileHandler.h"

SatFileHandler::~SatFileHandler(){
    
    
}

bool SatFileHandler::writef(string filenameBase, const char *message){

    std::fstream file; 
    std::string fileName = "fs/" + filenameBase;
    fileName += current + ".txt";

    file.open(fileName, std::ios::out | std::ios::app);

    if (!file.is_open()){
        if (pc){
            pc->printf("ERROR failed to open file");
        }

        return false;
    }

    file << message << std::endl;

    file.close();
    return true;
}

bool SatFileHandler::write(string filenameBase, string &message){
    return this->writef(filenameBase, message.c_str());
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