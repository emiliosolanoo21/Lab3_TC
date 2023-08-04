#include "forGraphic.h"
#include <string>
#include <stack>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>


TreeNode* constructTree(const std::string& postfixExpr) {
    std::stack<TreeNode*> st;

    for (char c : postfixExpr) {
        if (isalpha(c)) {
            st.push(new TreeNode(std::string(1, c)));
        } else if (c == '*' || c == '|') {
            TreeNode* right = st.top();
            st.pop();
            TreeNode* left = st.top();
            st.pop();
            st.push(new TreeNode(std::string(1, c)));
            st.top()->left = left;
            st.top()->right = right;
        } else if (c == ',') {
            TreeNode* right = st.top();
            st.pop();
            TreeNode* left = st.top();
            st.pop();
            st.top()->right = right;
            st.push(left);
        }
    }

    return st.top();
}
