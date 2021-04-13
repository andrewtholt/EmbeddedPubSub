#include <cstdint>

struct data {    
    ptypes type;    
    union  {    
        std::uint8_t a;    
        std::uint32_t b;    
        char c[32];    
    };    
};
