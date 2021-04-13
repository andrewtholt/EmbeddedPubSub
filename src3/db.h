#include <cstdint>

#include "enums.h"

#include "data.h"

class db {
    private:
    protected:
    public:
        db();
        const bool create(char *key, char *value); // default type is string
        const bool create(char *key, bool value);
        const bool create(char *key, uint8_t value);
        const bool create(char *key, uint32_t value);

        bool exists(char *key);

        const char *read(const char *key);
        const bool readBool(const char *key);
        const uint8_t readByte(const char *key);
        const uint32_t readInt(const char *key);
        const char *readString(const char *key);

        const bool update(const char *key, const char *value);
        const bool updateBool(const char *key, const bool value);
        const bool updateByte(const char *key, const uint8_t value);
        const bool updateInt(const char *key, const uint32_t value);
        const bool updateString(const char *key, const char *value);

        void rmData(const char *key);
};

