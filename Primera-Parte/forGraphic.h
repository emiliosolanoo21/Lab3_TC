#pragma once
#include <string>
#include <stack>

struct TreeNode {
    std::string data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(const std::string& val) : data(val), left(nullptr), right(nullptr) {}
};

TreeNode* constructTree(const std::string& postfixExpr);


