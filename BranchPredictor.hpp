#ifndef __BRANCH_PREDICTOR_HPP__
#define __BRANCH_PREDICTOR_HPP__

#include <vector>
#include <bitset>
#include <cassert>

struct BranchPredictor {
    virtual bool predict(uint32_t pc) = 0;
    virtual void update(uint32_t pc, bool taken) = 0;
};

struct SaturatingBranchPredictor : public BranchPredictor {
    std::vector<std::bitset<2>> table;
    SaturatingBranchPredictor(int value) : table(1 << 14, value) {}

    bool predict(uint32_t pc) {
        // your code here
        int table_index = pc & 0x3FFF;
        std:: bitset<2> saturating_counter = table[table_index];
        return saturating_counter.test(1);
    }

    void update(uint32_t pc, bool taken) {
        // your code here
        int table_index = pc & 0x3FFF;
        std:: bitset<2> saturating_counter = table[table_index];
        if (taken) {
            if (saturating_counter.to_ulong() < 3) {
                saturating_counter = saturating_counter.to_ulong() + 1;
            }
        } else {
            if (saturating_counter.to_ulong() > 0) {
                saturating_counter = saturating_counter.to_ulong() - 1;
            }
        }
        table[table_index] = saturating_counter;
    }
};

struct BHRBranchPredictor : public BranchPredictor {
    std::vector<std::bitset<2>> bhrTable;
    std::bitset<2> bhr;
    BHRBranchPredictor(int value) : bhrTable(1 << 2, value), bhr(value) {}

    bool predict(uint32_t pc) {
        // your code here
        int bhrTable_index = bhr.to_ulong();
        std:: bitset<2> saturating_counter = bhrTable[bhrTable_index];
        return saturating_counter.test(1);
    }

    void update(uint32_t pc, bool taken) {
        // your code here
        int bhrTable_index = bhr.to_ulong();
        std:: bitset<2> saturating_counter = bhrTable[bhrTable_index];
        if (taken) {
            if (saturating_counter.to_ulong() < 3) {
                saturating_counter = saturating_counter.to_ulong() + 1;
            }
        } else {
            if (saturating_counter.to_ulong() > 0) {
                saturating_counter = saturating_counter.to_ulong() - 1;
            }
        }
        bhrTable[bhrTable_index] = saturating_counter;
        bhr = bhr << 1;
        if (taken)
        {
            bhr.set(0);
        } 
    }
};

struct SaturatingBHRBranchPredictor : public BranchPredictor {
    std::vector<std::bitset<2>> bhrTable;
    std::bitset<2> bhr;
    std::vector<std::bitset<2>> table;
    std::vector<std::bitset<2>> combination;

    SaturatingBHRBranchPredictor(int value, int size) : bhrTable(1 << 2, value), bhr(value), table(1 << 14, value), combination(size, value) {
        assert(size <= (1 << 16));
    }

    bool predict(uint32_t pc) {
        // your code here
        int table_index = pc & 0x3FFF;
        int combinationIndex = (bhr.to_ulong() << 14) | table_index;
        std::bitset<2> saturating_counter = combination[combinationIndex];
        return saturating_counter[1];
    }

    void update(uint32_t pc, bool taken) {
        // your code here
        int table_index = pc & 0x3FFF;
        int combinationIndex = (bhr.to_ulong() << 14) | table_index;
        std::bitset<2> saturating_counter = combination[combinationIndex];
        if (taken) {
            if (saturating_counter.to_ulong() < 3) {
                saturating_counter = saturating_counter.to_ulong() + 1;
            }
        }
        else {
            if (saturating_counter.to_ulong() > 0) {
                saturating_counter = saturating_counter.to_ulong() - 1;
            }
        }
        combination[combinationIndex] = saturating_counter;
        bhr = bhr << 1;
        if (taken){
            bhr.set(0);
        }
    }
};


#endif