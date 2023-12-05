#ifndef INPUT_VERIFICATION_H
#define INPUT_VERIFICATION_H

#include <vector>
#include <string>

using namespace std;

bool is_unexpected_aid(string s);
bool is_unexpected_uid(string s);
bool is_unexpected_password(string s);

bool is_unexpected_login_input(vector<string> arguments);
bool is_unexpected_logout_input(vector<string> arguments);
bool is_unexpected_unregister_input(vector<string> arguments);
bool is_unexpected_list_auctions_target_input(vector<string> arguments);
bool is_unexpected_list_auctions_input(vector<string> arguments);
bool is_unexpected_list_bids_target_input(vector<string> arguments);

#endif
