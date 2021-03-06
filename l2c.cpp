#include "l2c.h"

#include "constants.h"
#include "main.h"
#include "clustermanager.h"

std::string L2C_Token::to_string(ClusterManager* cluster, uint64_t rel) const
{
    char tmp[1024];
    std::string out = "";
    for (size_t i = 0; fork_hierarchy.size() && i < fork_hierarchy.size() - 1; i++)
    {
        out += "  ";
    }

    //printf("%s%" PRIx64 " ", (rel ? "+" : ""), pc - rel);
    //if (rel)
    //    printf("b:%" PRIx64 "", cluster->block_hash(rel));
    //out += (rel ? "+" : "");
    snprintf(tmp, 1024, "%" PRIx64 " ", pc);
    out += std::string(tmp);

    //printf("%s", fork_hierarchy_str().c_str());
    out += " " + str;

    if (args.size())
        out += " args ";

    bool is_exit = false;
    if (str == "SUB_BRANCH" || str == "SUB_GOTO" || str == "DIV_FALSE" || str == "DIV_TRUE" || str == "CONV" || str == "BLOCK_MERGE" || str == "SPLIT_BLOCK_MERGE")
        is_exit = true;

    for (size_t i = 0; i < args.size(); i++)
    {
        if (is_exit && rel)
        {
            // Address difference
            /*bool neg = false;
            uint64_t val = args[i] - pc;
            if ((*(int64_t*)&val) < 0)
            {
                val = (val ^ ~0) + 1;
                neg = true;
            }

            snprintf(tmp, 1024, "%s0x%" PRIx64 "", (neg ? "-" : "+"), val);*/
            
            // Hash
            snprintf(tmp, 1024, "b:%" PRIx64 "", cluster->block_hash(args[i]));
            out += std::string(tmp);
        }
        else
        {
            snprintf(tmp, 1024, "0x%" PRIx64 "", args[i]);
            out += std::string(tmp);
            
            if (unhash[args[i]] != "")
                out += " (" + unhash[args[i]] + ")";
            for (auto j : arg_is_const_value) {
                if (arg_is_const_value[j] == i) {
                    out += " (" + const_value_table[args[i]] + ")";
                }
            }
        }

        if (i < args.size() - 1)
            out += ", ";
    }

    if (fargs.size())
        out += " fargs ";

    for (auto i = 0; i < fargs.size(); i++)
    {
        snprintf(tmp, 1024, "%f", fargs[i]);
        out += std::string(tmp);
        if (i < fargs.size() - 1)
            out += ", ";
    }

    out += "\n";
    return out;
}

void L2C_Token::to_file(ClusterManager* cluster, uint64_t rel, std::ofstream& file) const
{
    char tmp[1024];
    std::string out = "";
    for (size_t i = 0; fork_hierarchy.size() && i < fork_hierarchy.size() - 1; i++)
    {
        file << "  ";
    }

    snprintf(tmp, 1024, "%" PRIx64 " ", pc);
    file << std::string(tmp);

    //printf("%s", fork_hierarchy_str().c_str());
    file << " " + str;

    if (args.size())
        file << " args ";

    bool is_exit = false;
    if (str == "SUB_BRANCH" || str == "SUB_GOTO" || str == "DIV_FALSE" || str == "DIV_TRUE" || str == "CONV" || str == "BLOCK_MERGE" || str == "SPLIT_BLOCK_MERGE")
        is_exit = true;

    for (size_t i = 0; i < args.size(); i++)
    {
        if (is_exit && rel)
        {
            // Address difference
            /*bool neg = false;
            uint64_t val = args[i] - pc;
            if ((*(int64_t*)&val) < 0)
            {
                val = (val ^ ~0) + 1;
                neg = true;
            }

            snprintf(tmp, 1024, "%s0x%" PRIx64 "", (neg ? "-" : "+"), val);*/
            
            // Hash
            snprintf(tmp, 1024, "b:%" PRIx64 "", cluster->block_hash(args[i]));
            file << std::string(tmp);
        }
        else
        {
            snprintf(tmp, 1024, "0x%" PRIx64 "", args[i]);
            file << std::string(tmp);
            
            if (unhash[args[i]] != "")
                file << " (" + unhash[args[i]] + ")";
            for (auto j : arg_is_const_value) {
                if (arg_is_const_value[j] == i) {
                    file << " (" + const_value_table[args[i]] + ")";
                    break;
                }
            }
        }

        if (i < args.size() - 1)
            file << ", ";
    }

    if (fargs.size())
        file << " fargs ";

    for (auto i = 0; i < fargs.size(); i++)
    {
        snprintf(tmp, 1024, "%f", fargs[i]);
        file << std::string(tmp);
        if (i < fargs.size() - 1)
            file << ", ";
    }

    return;
}
