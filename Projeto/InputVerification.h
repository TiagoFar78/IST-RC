#ifndef INPUT_VERIFICATION_H
#define INPUT_VERIFICATION_H

#include <vector>
#include <string>

using namespace std;

bool is_unexpected_aid(string s);
bool is_unexpected_uid(string s);
bool is_unexpected_password(string s);
bool is_unexpected_value(string value);
bool is_unexpected_name(string name);
bool is_unexpected_timeactive(string time);
bool is_unexpected_fname(string fname);
bool is_unexpected_fsize(string fsize);

bool is_unexpected_login_input(vector<string> arguments);
bool is_unexpected_logout_input(vector<string> arguments);
bool is_unexpected_unregister_input(vector<string> arguments);
bool is_unexpected_list_auctions_target_input(vector<string> arguments);
bool is_unexpected_list_auctions_input(vector<string> arguments);
bool is_unexpected_list_bids_target_input(vector<string> arguments);
bool is_unexpected_show_record_input(vector<string> arguments);
bool is_unexpected_close_input(vector<string> arguments, bool is_from_client);
bool is_unexpected_show_asset_input(vector<string> arguments);
bool is_unexpected_bid_input(vector<string> arguments, bool is_from_client);
bool is_unexpected_open_input(vector<string> arguments, bool is_from_client)


#endif
