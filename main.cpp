#include <iostream>
#include "Base/complex.h"
#include "Base/signalFile.h"
#include "Base/reader.h"
#include "Base/demod.h"
#include "Base/writer.h"

int main()
{

   // Complex <int32_t> lolo (3, 4);
   //std::cout << lolo.conj().im() << std::endl;
    Reader reader;
    std::string ds = "/home/ann/WORK/work_qt/firstSt/demodulDD//dataset66";
    SignalFile<float> fileM = reader.readFile<float>(ds);
 /*   for(uint32_t i=0;i<fileM.size();i++){
        std::cout << fileM.data()[i].re() << '\t' << fileM.data()[i].im() << std::endl;
    }
*/
    Demodulation demo;
    std:: vector<float> data = demo.demodAM(fileM);
    int64_t fd = fileM.freqSample();
    Writer writer;
    std::string directory = "/home/ann/WORK/work_mtlb/";
    writer.writeFile(directory, data, fd);
    return 0;


}
