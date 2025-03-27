/*
 * Copyright (C) Mohsen Zohrevandi, 2017
 *               Rida Bazzi 2019
 * Do not share this file with anyone
 */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <set>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include "lexer.h"

using namespace std;

// Rule structure to represent a grammar rule
struct Rule {
    string lhs;
    vector<string> rhs;

    // Equality operator for rules
    bool operator==(const Rule& other) const {
        if (lhs != other.lhs) return false;
        if (rhs.size() != other.rhs.size()) return false;
        for (size_t i = 0; i < rhs.size(); i++) {
            if (rhs[i] != other.rhs[i]) return false;
        }
        return true;
    }
};

// Global variables
vector<Rule> grammar;
vector<string> terminals;
vector<string> non_terminals;
set<string> terminalSet;
set<string> nonTerminalSet;

// read grammar
void ReadGrammar() {
    LexicalAnalyzer lexer;
    Token token;
    
    // Process rules until we reach the end of the grammar (HASH)
    while (true) {
        // Get the left-hand side (a non-terminal)
        token = lexer.GetToken();
        if (token.token_type == HASH) {
            break;
        }
        
        if (token.token_type != ID) {
            cout << "SYNTAX ERROR !!!!!!!!!!!!!!!" << endl;
            exit(1);
        }
        
        string lhs = token.lexeme;
        
        // Add to non-terminals if not already there
        if (nonTerminalSet.find(lhs) == nonTerminalSet.end()) {
            nonTerminalSet.insert(lhs);
            non_terminals.push_back(lhs);
        }
        
        // Check for ARROW token
        token = lexer.GetToken();
        if (token.token_type != ARROW) {
            cout << "SYNTAX ERROR !!!!!!!!!!!!!!!" << endl;
            exit(1);
        }
        
        // Process right-hand sides (possibly multiple alternatives)
        while (true) {
            vector<string> rhs;
            
            // Parse symbols until OR or STAR
            token = lexer.GetToken();
            while (token.token_type == ID) {
                string symbol = token.lexeme;
                rhs.push_back(symbol);
                
                // Track symbols (potentially terminals)
                if (terminalSet.find(symbol) == terminalSet.end() && 
                    nonTerminalSet.find(symbol) == nonTerminalSet.end()) {
                    terminalSet.insert(symbol);
                    terminals.push_back(symbol);
                }
                
                token = lexer.GetToken();
            }
            
            // Create a rule with this alternative
            Rule rule;
            rule.lhs = lhs;
            rule.rhs = rhs;
            grammar.push_back(rule);
            
            // Check if we're at the end of the rule or have another alternative
            if (token.token_type == STAR) {
                break;
            } else if (token.token_type == OR) {
                continue;
            } else {
                cout << "SYNTAX ERROR !!!!!!!!!!!!!!!" << endl;
                exit(1);
            }
        }
    }
    
    // Remove non-terminals from the terminals list
    auto it = terminals.begin();
    while (it != terminals.end()) {
        if (nonTerminalSet.find(*it) != nonTerminalSet.end()) {
            terminalSet.erase(*it);
            it = terminals.erase(it);
        } else {
            ++it;
        }
    }
}

/* 
 * Task 1: 
 * Printing the terminals, then nonterminals of grammar in appearing order
 * output is one line, and all names are space delineated
*/
void Task1() {
    // Print terminals
    for (const string& terminal : terminals) {
        cout << terminal << " ";
    }

    // Print non-terminals
    for (const string& non_terminal : non_terminals) {
        cout << non_terminal << " ";
    }

    cout << endl;
}

/*
 * Task 2:
 * Print out nullable set of the grammar in specified format.
*/
void Task2() {
    // Calculate nullable non-terminals
    set<string> nullable;
    
    // Initialization: Add all non-terminals with epsilon rules
    for (const Rule& rule : grammar) {
        if (rule.rhs.empty()) {
            nullable.insert(rule.lhs);
        }
    }
    
    // Iteratively find more nullable non-terminals
    bool changed = true;
    while (changed) {
        changed = false;
        
        for (const Rule& rule : grammar) {
            // Skip if already nullable
            if (nullable.find(rule.lhs) != nullable.end()) {
                continue;
            }
            
            // Check if all symbols on the RHS are nullable
            bool all_nullable = true;
            for (const string& symbol : rule.rhs) {
                if (nullable.find(symbol) == nullable.end()) {
                    all_nullable = false;
                    break;
                }
            }
            
            // If all symbols are nullable, the LHS is nullable
            if (all_nullable && !rule.rhs.empty()) {
                nullable.insert(rule.lhs);
                changed = true;
            }
        }
    }
    
    // Print in order of appearance in the grammar
    cout << "Nullable = { ";
    bool first = true;
    for (const string& non_terminal : non_terminals) {
        if (nullable.find(non_terminal) != nullable.end()) {
            if (!first) {
                cout << ", ";
            }
            cout << non_terminal;
            first = false;
        }
    }
    cout << " }" << endl;
}

// Task 3: FIRST sets
void Task3()
{
    // First, calculate nullable non-terminals
    set<string> nullable;
    for (const Rule& rule : grammar) {
        if (rule.rhs.empty()) {
            nullable.insert(rule.lhs);
        }
    }
    
    bool nullable_changed = true;
    while (nullable_changed) {
        nullable_changed = false;
        for (const Rule& rule : grammar) {
            if (nullable.find(rule.lhs) != nullable.end()) {
                continue;
            }
            
            bool all_nullable = true;
            for (const string& symbol : rule.rhs) {
                if (nullable.find(symbol) == nullable.end()) {
                    all_nullable = false;
                    break;
                }
            }
            
            if (all_nullable && !rule.rhs.empty()) {
                nullable.insert(rule.lhs);
                nullable_changed = true;
            }
        }
    }
    
    // Initialize FIRST sets
    unordered_map<string, set<string>> FIRST;
    
    // Initialize FIRST sets for terminals
    for (const string& terminal : terminals) {
        FIRST[terminal].insert(terminal);
    }
    
    // Iteratively calculate FIRST sets
    bool first_changed = true;
    while (first_changed) {
        first_changed = false;
        
        for (const Rule& rule : grammar) {
            if (rule.rhs.empty()) {
                continue;  // Skip epsilon rules
            }
            
            // Apply Rule II: If A -> B C... then add FIRST(B) to FIRST(A)
            for (size_t i = 0; i < rule.rhs.size(); i++) {
                const string& symbol = rule.rhs[i];
                
                // Add FIRST(symbol) to FIRST(rule.lhs)
                set<string>& first_lhs = FIRST[rule.lhs];
                set<string>& first_symbol = FIRST[symbol];
                size_t old_size = first_lhs.size();
                
                first_lhs.insert(first_symbol.begin(), first_symbol.end());
                
                if (first_lhs.size() > old_size) {
                    first_changed = true;
                }
                
                // If this symbol is not nullable, stop
                if (nullable.find(symbol) == nullable.end()) {
                    break;
                }
                
                // If we're at the last symbol and it's nullable, we're done with this rule
                if (i == rule.rhs.size() - 1) {
                    // No more symbols to process
                }
            }
        }
    }
    
    // Print FIRST sets in order of non-terminals
    for (const string& non_terminal : non_terminals) {
        cout << "FIRST(" << non_terminal << ") = { ";
        
        // Print elements in order of appearance
        vector<string> first_elements;
        for (const string& element : FIRST[non_terminal]) {
            first_elements.push_back(element);
        }
        
        // Sort elements by their order in the terminals vector
        vector<string> ordered_elements;
        for (const string& terminal : terminals) {
            if (find(first_elements.begin(), first_elements.end(), terminal) != first_elements.end()) {
                ordered_elements.push_back(terminal);
            }
        }
        
        // Print elements
        for (size_t i = 0; i < ordered_elements.size(); i++) {
            if (i > 0) {
                cout << ", ";
            }
            cout << ordered_elements[i];
        }
        
        cout << " }" << endl;
    }
}

// Task 4: FOLLOW sets
void Task4()
{
    // First, calculate nullable non-terminals
    set<string> nullable;
    for (const Rule& rule : grammar) {
        if (rule.rhs.empty()) {
            nullable.insert(rule.lhs);
        }
    }
    
    bool nullable_changed = true;
    while (nullable_changed) {
        nullable_changed = false;
        for (const Rule& rule : grammar) {
            if (nullable.find(rule.lhs) != nullable.end()) {
                continue;
            }
            
            bool all_nullable = true;
            for (const string& symbol : rule.rhs) {
                if (nullable.find(symbol) == nullable.end()) {
                    all_nullable = false;
                    break;
                }
            }
            
            if (all_nullable && !rule.rhs.empty()) {
                nullable.insert(rule.lhs);
                nullable_changed = true;
            }
        }
    }
    
    // Calculate FIRST sets
    unordered_map<string, set<string>> FIRST;
    
    // Initialize FIRST sets for terminals
    for (const string& terminal : terminals) {
        FIRST[terminal].insert(terminal);
    }
    
    // Iteratively calculate FIRST sets
    bool first_changed = true;
    while (first_changed) {
        first_changed = false;
        
        for (const Rule& rule : grammar) {
            if (rule.rhs.empty()) {
                continue;
            }
            
            for (size_t i = 0; i < rule.rhs.size(); i++) {
                const string& symbol = rule.rhs[i];
                
                set<string>& first_lhs = FIRST[rule.lhs];
                set<string>& first_symbol = FIRST[symbol];
                size_t old_size = first_lhs.size();
                
                first_lhs.insert(first_symbol.begin(), first_symbol.end());
                
                if (first_lhs.size() > old_size) {
                    first_changed = true;
                }
                
                if (nullable.find(symbol) == nullable.end()) {
                    break;
                }
                
                if (i == rule.rhs.size() - 1) {
                    // No more symbols to process
                }
            }
        }
    }
    
    // Initialize FOLLOW sets
    unordered_map<string, set<string>> FOLLOW;
    
    // Initialize FOLLOW set for start symbol
    if (!non_terminals.empty()) {
        FOLLOW[non_terminals[0]].insert("$");
    }
    
    // Function to get FIRST of a sequence of symbols
    auto getFirstOfSequence = [&](const vector<string>& sequence, size_t start) -> set<string> {
        set<string> result;
        
        for (size_t i = start; i < sequence.size(); i++) {
            const string& symbol = sequence[i];
            
            // Add FIRST(symbol) to the result
            result.insert(FIRST[symbol].begin(), FIRST[symbol].end());
            
            // If this symbol is not nullable, stop
            if (nullable.find(symbol) == nullable.end()) {
                break;
            }
            
            // If we're at the last symbol and all previous are nullable, we're done
            if (i == sequence.size() - 1) {
                // No more symbols to process
            }
        }
        
        return result;
    };
    
    // First pass: Apply rules IV and V
    for (const Rule& rule : grammar) {
        for (size_t i = 0; i < rule.rhs.size(); i++) {
            const string& symbol = rule.rhs[i];
            
            // Skip terminals
            if (nonTerminalSet.find(symbol) == nonTerminalSet.end()) {
                continue;
            }
            
            // If there are symbols after this one, add FIRST of the sequence to FOLLOW
            if (i < rule.rhs.size() - 1) {
                set<string> first_sequence = getFirstOfSequence(rule.rhs, i + 1);
                FOLLOW[symbol].insert(first_sequence.begin(), first_sequence.end());
            }
        }
    }
    
    // Iteratively apply rules II and III
    bool follow_changed = true;
    while (follow_changed) {
        follow_changed = false;
        
        for (const Rule& rule : grammar) {
            // Apply rules II and III
            for (size_t i = 0; i < rule.rhs.size(); i++) {
                const string& symbol = rule.rhs[i];
                
                // Skip terminals
                if (nonTerminalSet.find(symbol) == nonTerminalSet.end()) {
                    continue;
                }
                
                // Rule II: If A -> alpha B, add FOLLOW(A) to FOLLOW(B)
                if (i == rule.rhs.size() - 1) {
                    set<string>& follow_symbol = FOLLOW[symbol];
                    set<string>& follow_lhs = FOLLOW[rule.lhs];
                    size_t old_size = follow_symbol.size();
                    
                    follow_symbol.insert(follow_lhs.begin(), follow_lhs.end());
                    
                    if (follow_symbol.size() > old_size) {
                        follow_changed = true;
                    }
                }
                // Rule III: If A -> alpha B beta and beta is nullable, add FOLLOW(A) to FOLLOW(B)
                else {
                    bool all_nullable = true;
                    for (size_t j = i + 1; j < rule.rhs.size(); j++) {
                        if (nullable.find(rule.rhs[j]) == nullable.end()) {
                            all_nullable = false;
                            break;
                        }
                    }
                    
                    if (all_nullable) {
                        set<string>& follow_symbol = FOLLOW[symbol];
                        set<string>& follow_lhs = FOLLOW[rule.lhs];
                        size_t old_size = follow_symbol.size();
                        
                        follow_symbol.insert(follow_lhs.begin(), follow_lhs.end());
                        
                        if (follow_symbol.size() > old_size) {
                            follow_changed = true;
                        }
                    }
                }
            }
        }
    }
    
    // Print FOLLOW sets in order of non-terminals
    for (const string& non_terminal : non_terminals) {
        cout << "FOLLOW(" << non_terminal << ") = { ";
        
        // First, check if $ is in the FOLLOW set
        bool has_eof = FOLLOW[non_terminal].find("$") != FOLLOW[non_terminal].end();
        
        // Prepare list of elements excluding $
        vector<string> follow_elements;
        for (const string& element : FOLLOW[non_terminal]) {
            if (element != "$") {
                follow_elements.push_back(element);
            }
        }
        
        // Sort elements by their order in the terminals vector
        vector<string> ordered_elements;
        for (const string& terminal : terminals) {
            if (find(follow_elements.begin(), follow_elements.end(), terminal) != follow_elements.end()) {
                ordered_elements.push_back(terminal);
            }
        }
        
        // Print $ first if it exists
        if (has_eof) {
            cout << "$";
            if (!ordered_elements.empty()) {
                cout << ", ";
            }
        }
        
        // Print the rest of the elements
        for (size_t i = 0; i < ordered_elements.size(); i++) {
            if (i > 0) {
                cout << ", ";
            }
            cout << ordered_elements[i];
        }
        
        cout << " }" << endl;
    }
}

// Helper function for Task 5: Compare rules lexicographically
bool compareLexicographically(const Rule& rule1, const Rule& rule2) {
    if (rule1.lhs != rule2.lhs) {
        return rule1.lhs < rule2.lhs;
    }
    
    size_t min_size = min(rule1.rhs.size(), rule2.rhs.size());
    for (size_t i = 0; i < min_size; i++) {
        if (rule1.rhs[i] != rule2.rhs[i]) {
            return rule1.rhs[i] < rule2.rhs[i];
        }
    }
    
    return rule1.rhs.size() < rule2.rhs.size();
}

// Helper function for Task 5: Get the length of the common prefix of two rules
size_t commonPrefixLength(const vector<string>& rhs1, const vector<string>& rhs2) {
    size_t min_size = min(rhs1.size(), rhs2.size());
    size_t i;
    for (i = 0; i < min_size; i++) {
        if (rhs1[i] != rhs2[i]) {
            break;
        }
    }
    return i;
}

// Task 5: left factoring
void Task5() {
    // Start with the initial grammar
    vector<Rule> result = grammar;
    
    // Keep track of counter for new non-terminals
    unordered_map<string, int> counters;
    for (const string& nt : non_terminals) {
        counters[nt] = 1;
    }
    
    // Process until no more left factoring can be done
    bool changed = true;
    while (changed) {
        changed = false;
        
        // Group rules by their LHS
        unordered_map<string, vector<Rule>> rulesByLHS;
        for (const Rule& rule : result) {
            rulesByLHS[rule.lhs].push_back(rule);
        }
        
        // For each non-terminal, check if left factoring is needed
        for (const auto& pair : rulesByLHS) {
            const string& lhs = pair.first;
            const vector<Rule>& rules = pair.second;
            
            // Need at least 2 rules to consider left factoring
            if (rules.size() < 2) {
                continue;
            }
            
            // Find the longest common prefix among any two rules
            size_t max_prefix_length = 0;
            vector<Rule> rules_with_prefix;
            vector<string> max_prefix;
            
            for (size_t i = 0; i < rules.size(); i++) {
                for (size_t j = i + 1; j < rules.size(); j++) {
                    size_t prefix_length = commonPrefixLength(rules[i].rhs, rules[j].rhs);
                    if (prefix_length > 0 && (prefix_length > max_prefix_length || 
                        (prefix_length == max_prefix_length && 
                         lexicographical_compare(rules[i].rhs.begin(), rules[i].rhs.begin() + prefix_length,
                                               max_prefix.begin(), max_prefix.end())))) {
                        max_prefix_length = prefix_length;
                        max_prefix.clear();
                        for (size_t k = 0; k < prefix_length; k++) {
                            max_prefix.push_back(rules[i].rhs[k]);
                        }
                        
                        rules_with_prefix.clear();
                        // Collect all rules with this prefix
                        for (const Rule& rule : rules) {
                            if (rule.rhs.size() >= prefix_length) {
                                bool matches = true;
                                for (size_t k = 0; k < prefix_length; k++) {
                                    if (rule.rhs[k] != rules[i].rhs[k]) {
                                        matches = false;
                                        break;
                                    }
                                }
                                if (matches) {
                                    rules_with_prefix.push_back(rule);
                                }
                            }
                        }
                    }
                }
            }
            
            // If we found a common prefix, left factor it
            if (max_prefix_length > 0) {
                changed = true;
                
                // Create a new non-terminal
                string new_nt = lhs + to_string(counters[lhs]);
                counters[lhs]++;
                
                // Create the left-factored rule
                Rule factored_rule;
                factored_rule.lhs = lhs;
                for (size_t i = 0; i < max_prefix_length; i++) {
                    factored_rule.rhs.push_back(rules_with_prefix[0].rhs[i]);
                }
                factored_rule.rhs.push_back(new_nt);
                
                // Remove the old rules with the common prefix and add the new one
                vector<Rule> new_result;
                for (const Rule& rule : result) {
                    bool found = false;
                    for (const Rule& r : rules_with_prefix) {
                        if (rule == r) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        new_result.push_back(rule);
                    }
                }
                new_result.push_back(factored_rule);
                
                // Add rules for the new non-terminal
                for (const Rule& rule : rules_with_prefix) {
                    Rule new_rule;
                    new_rule.lhs = new_nt;
                    for (size_t i = max_prefix_length; i < rule.rhs.size(); i++) {
                        new_rule.rhs.push_back(rule.rhs[i]);
                    }
                    new_result.push_back(new_rule);
                }
                
                result = new_result;
                break;  // Start over with the new grammar
            }
        }
    }
    
    // Sort the resulting grammar lexicographically
    sort(result.begin(), result.end(), compareLexicographically);
    
    // Print the result
    for (const Rule& rule : result) {
        cout << rule.lhs << " -> ";
        if (rule.rhs.empty()) {
            cout << "";
        } else {
            for (size_t i = 0; i < rule.rhs.size(); i++) {
                cout << rule.rhs[i] << " ";
            }
        }
        cout << "#" << endl;
    }
}

// Task 6: eliminate left recursion
void Task6() {
    // Start with the original grammar
    vector<Rule> result = grammar;
    
    // Sort non-terminals lexicographically
    vector<string> sorted_nt = non_terminals;
    sort(sorted_nt.begin(), sorted_nt.end());
    
    // Keep track of counter for new non-terminals
    unordered_map<string, int> counters;
    for (const string& nt : non_terminals) {
        counters[nt] = 1;
    }
    
    // For each non-terminal in the sorted order
    for (size_t i = 0; i < sorted_nt.size(); i++) {
        const string& A_i = sorted_nt[i];
        
        // Group rules by their LHS
        unordered_map<string, vector<Rule>> rulesByLHS;
        for (const Rule& rule : result) {
            rulesByLHS[rule.lhs].push_back(rule);
        }
        
        // For each non-terminal that precedes A_i
        for (size_t j = 0; j < i; j++) {
            const string& A_j = sorted_nt[j];
            
            // For each rule with A_i on the left-hand side
            vector<Rule> new_A_i_rules;
            for (const Rule& rule : rulesByLHS[A_i]) {
                // If the first symbol of the rule is A_j, replace it
                if (!rule.rhs.empty() && rule.rhs[0] == A_j) {
                    // For each rule with A_j on the left-hand side
                    for (const Rule& rule_j : rulesByLHS[A_j]) {
                        // Create a new rule by replacing A_j with its RHS
                        Rule new_rule;
                        new_rule.lhs = A_i;
                        
                        // Add RHS of rule_j
                        for (const string& symbol : rule_j.rhs) {
                            new_rule.rhs.push_back(symbol);
                        }
                        
                        // Add the rest of the original rule
                        for (size_t k = 1; k < rule.rhs.size(); k++) {
                            new_rule.rhs.push_back(rule.rhs[k]);
                        }
                        
                        new_A_i_rules.push_back(new_rule);
                    }
                } else {
                    // Keep the rule as is
                    new_A_i_rules.push_back(rule);
                }
            }
            
            // Replace the old rules with the new ones
            vector<Rule> temp_result;
            for (const Rule& rule : result) {
                if (rule.lhs != A_i) {
                    temp_result.push_back(rule);
                }
            }
            for (const Rule& rule : new_A_i_rules) {
                temp_result.push_back(rule);
            }
            result = temp_result;
            
            // Update rulesByLHS
            rulesByLHS.clear();
            for (const Rule& rule : result) {
                rulesByLHS[rule.lhs].push_back(rule);
            }
        }
        
        // Now eliminate direct left recursion from A_i
        vector<Rule> A_i_alpha;  // Rules A_i -> A_i alpha
        vector<Rule> A_i_beta;   // Rules A_i -> beta
        
        for (const Rule& rule : rulesByLHS[A_i]) {
            if (!rule.rhs.empty() && rule.rhs[0] == A_i) {
                A_i_alpha.push_back(rule);
            } else {
                A_i_beta.push_back(rule);
            }
        }
        
        // If there is direct left recursion
        if (!A_i_alpha.empty()) {
            // Create a new non-terminal A_i1
            string A_i1 = A_i + to_string(counters[A_i]);
            counters[A_i]++;
            
            // Replace the rules
            vector<Rule> temp_result;
            for (const Rule& rule : result) {
                if (rule.lhs != A_i) {
                    temp_result.push_back(rule);
                }
            }
            
            // Add A_i -> beta A_i1
            for (const Rule& rule : A_i_beta) {
                Rule new_rule;
                new_rule.lhs = A_i;
                for (const string& symbol : rule.rhs) {
                    new_rule.rhs.push_back(symbol);
                }
                new_rule.rhs.push_back(A_i1);
                temp_result.push_back(new_rule);
            }
            
            // Add A_i1 -> alpha A_i1
            for (const Rule& rule : A_i_alpha) {
                Rule new_rule;
                new_rule.lhs = A_i1;
                for (size_t j = 1; j < rule.rhs.size(); j++) {
                    new_rule.rhs.push_back(rule.rhs[j]);
                }
                new_rule.rhs.push_back(A_i1);
                temp_result.push_back(new_rule);
            }
            
            // Add A_i1 -> epsilon
            Rule epsilon_rule;
            epsilon_rule.lhs = A_i1;
            // Leave RHS empty for epsilon
            temp_result.push_back(epsilon_rule);
            
            result = temp_result;
        }
    }
    
    // Sort the resulting grammar lexicographically
    sort(result.begin(), result.end(), compareLexicographically);
    
    // Print the result
    for (const Rule& rule : result) {
        cout << rule.lhs << " -> ";
        if (rule.rhs.empty()) {
            cout << "";
        } else {
            for (size_t i = 0; i < rule.rhs.size(); i++) {
                cout << rule.rhs[i] << " ";
            }
        }
        cout << "#" << endl;
    }
}
    
int main (int argc, char* argv[])
{
    int task;

    if (argc < 2)
    {
        cout << "Error: missing argument\n";
        return 1;
    }

    /*
       Note that by convention argv[0] is the name of your executable,
       and the first argument to your program is stored in argv[1]
     */

    task = atoi(argv[1]);
    
    ReadGrammar();  // Reads the input grammar from standard input
                    // and represent it internally in data structures
                    // ad described in project 2 presentation file

    switch (task) {
        case 1: Task1();
            break;

        case 2: Task2();
            break;

        case 3: Task3();
            break;

        case 4: Task4();
            break;

        case 5: Task5();
            break;
        
        case 6: Task6();
            break;

        default:
            cout << "Error: unrecognized task number " << task << "\n";
            break;
    }
    return 0;
}