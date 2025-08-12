#ifndef LOC_H
#define LOC_H

#include <utility>

struct Loc2{
    int x;
    int z;

    inline bool operator==(const Loc2& loc2) const {
        return x==loc2.x && z==loc2.z;
    }
    inline bool operator<(const Loc2& loc2) const {
        if(x!=loc2.x)
            return x<loc2.x;
        return  z<loc2.z;
    }
};

//Tweaked the chatgpt hash for size two struct
namespace std {
    template <>
    struct hash<Loc2> {
        std::size_t operator()(const Loc2& loc) const {
            std::size_t h1 = std::hash<int>{}(loc.x);
            std::size_t h3 = std::hash<int>{}(loc.z);

            // Mix the three hash values
            std::size_t h = h1;
            h ^= h3 + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };
}

struct LocInt{
    int x;
    int y;
    int z;

    inline bool operator==(const LocInt& loc2) const {
        return x==loc2.x && y==loc2.y && z==loc2.z;
    }
};

// I copied this hash function from chatgpt
// It uses a golden ratio constant or something.
namespace std {
    template <>
    struct hash<LocInt> {
        std::size_t operator()(const LocInt& loc) const {
            std::size_t h1 = std::hash<int>{}(loc.x);
            std::size_t h2 = std::hash<int>{}(loc.y);
            std::size_t h3 = std::hash<int>{}(loc.z);

            // Mix the three hash values
            std::size_t h = h1;
            h ^= h2 + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= h3 + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };
}


struct LocFloat{
    float x;
    float y;
    float z;

    inline bool operator==(const LocFloat& loc2) const {
        return x==loc2.x && y==loc2.y && z==loc2.z;
    }
};

// I again copied this hash function from chatgpt
// It uses a golden ratio constant or something.
namespace std {
    template <>
    struct hash<LocFloat> {
        std::size_t operator()(const LocFloat& loc) const {
            std::size_t h1 = std::hash<float>{}(loc.x);
            std::size_t h2 = std::hash<float>{}(loc.y);
            std::size_t h3 = std::hash<float>{}(loc.z);

            // Mix the three hash values
            std::size_t h = h1;
            h ^= h2 + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= h3 + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };
}

inline const LocInt dirs[] = {{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}};

inline LocInt operator+(const LocInt& loc1, const LocInt& loc2){
    return {loc1.x+loc2.x, loc1.y+loc2.y, loc1.z+loc2.z};
}

inline LocFloat operator+(const LocInt& loc1, const LocFloat& loc2){
    return {loc1.x+loc2.x, loc1.y+loc2.y, loc1.z+loc2.z};
}

inline LocFloat operator+(const LocFloat& loc1, const LocInt& loc2){
    return {loc1.x+loc2.x, loc1.y+loc2.y, loc1.z+loc2.z};
}

inline LocFloat operator+(const LocFloat& loc1, const LocFloat& loc2){
    return {loc1.x+loc2.x, loc1.y+loc2.y, loc1.z+loc2.z};
}

typedef Loc2 ChunkID;


#endif //LOC_H