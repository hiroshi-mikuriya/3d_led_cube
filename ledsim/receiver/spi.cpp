#include "led.h"
#include "spi.h"
#include <stdexcept>
#include <vector>
#include <bcm2835.h>

using namespace makerfaire::fxat;

namespace
{
    const uint8_t WriteSpi = 0x02;
    const unsigned short AddrFpgaFirst = 0x0000;
    const unsigned short AddrFpgaSecond = 0x0C00;
    const unsigned short AddrFpgaThird = 0x1800;
    const unsigned short AddrFpgaFourth = 0x2400;
    const unsigned short AddrFpgaEnd = 0x3000;
    
    enum LedColor {
        Red = 0,
        Green,
        Blue,
    };
    
    unsigned short CalcAddr(int x, int y, int z, LedColor ledColor = Red)
    {
        int addr;
        if (!(0 <= x && x < 16) || !(0 <= y && y < 32) || !(0 <= z && z < 8))
        {
            throw std::runtime_error("x * y * z must be set between 0 to 4095.");
        }
        
        switch (ledColor)
        {
            case Red:
                addr = x * 3;
                break;
            case Green:
                addr = x * 3 + 1;
                break;
            case Blue:
                addr = x * 3 + 2;
                break;
            default:
                throw std::runtime_error("The ledColor must be set any of the Red(0) or Green(1) or Blue(2).");
                break;
        }
        
        addr += y * makerfaire::fxat::Led::Width * 3;
        addr += z * makerfaire::fxat::Led::Width * makerfaire::fxat::Led::Height * 3;
        
        return addr;
    }
    
    
    void Transfer(std::vector<std::vector<uint8_t>> spiData)
    {
        auto proc = [](std::vector<uint8_t> const & spiData)
        {
            uint8_t * tx = const_cast<uint8_t*>(spiData.data());
            uint8_t * rx = new uint8_t[spiData.size()];
            bcm2835_spi_transfernb((char*)tx, (char*)rx, spiData.size());
            for (size_t i = 0; i < spiData.size(); ++i){
                if (tx[i] != rx[i]){
                    //std::cout << "Can't send or receive presisely." << std::endl;
                }
            }
            delete rx;
        };
        
        if (!bcm2835_init()){
            return;
        }
        
        bcm2835_spi_begin();
        bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
        bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default. Clock polarity = 0(Positive), Clock phase = 0(Positive)
        bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64); // Not default. 32 = 128ns = 7.8125MHz
        //bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64); // Not default. Divide 64 = 256ns = 3.90625MHz
        bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
        bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default
        
        for (size_t i = 0; i < spiData.size(); ++i){
            proc(spiData[i]);
        }
        
        bcm2835_spi_end();
        bcm2835_close();
    }
}

void SendSpi(int const (&led)[makerfaire::fxat::Led::Width][makerfaire::fxat::Led::Height][makerfaire::fxat::Led::Depth])
{
    auto makeSpiDataBurst = [&led]()->std::vector<std::vector<uint8_t>>
    {
        auto shiftBit = [](int rgb, LedColor ledColor) -> uint8_t {
            uint8_t shifted;
            switch (ledColor){
                case Red:
                    shifted = static_cast<uint8_t>(rgb >> 16);
                    break;
                case Green:
                    shifted = static_cast<uint8_t>(rgb >> 8);
                    break;
                case Blue:
                    shifted = static_cast<uint8_t>(rgb >> 0);
                    break;
                default:
                    throw std::runtime_error("The ledColor must be set any of the Red(0) or Green(1) or Blue(2).");
                    break;
            }
            return shifted;
        };
        std::vector<std::vector<uint8_t>> res;
        for (int z = 0; z < Led::Depth; ++z){
            std::vector<uint8_t> tmpData;
            tmpData.push_back(WriteSpi);
            unsigned short addrBegin = CalcAddr(0, 0, z);
            tmpData.push_back(static_cast<uint8_t>(addrBegin >> 8));	//Big Endian (High bits)
            tmpData.push_back(static_cast<uint8_t>(addrBegin >> 0));	//Big Endian (Low bits)
            for (int y = 0; y < Led::Height; ++y){
                for (int x = 0; x < Led::Width; ++x){
                    int rgb = led[x][y][z];
                    tmpData.push_back(shiftBit(rgb, Red));
                    tmpData.push_back(shiftBit(rgb, Green));
                    tmpData.push_back(shiftBit(rgb, Blue));
                }
            }
            res.push_back(tmpData);
        }
        return res;
    };
    Transfer(makeSpiDataBurst());
}
