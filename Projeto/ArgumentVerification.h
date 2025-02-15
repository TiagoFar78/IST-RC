#ifndef ARGUMENT_VERIFICATION_H
#define ARGUMENT_VERIFICATION_H

#include <vector>
#include <string>

using namespace std;

bool is_unexpected_login_input(vector<string> arguments, bool is_from_client);
bool is_unexpected_logout_input(vector<string> arguments, bool is_from_client);
bool is_unexpected_unregister_input(vector<string> arguments, bool is_from_client);
bool is_unexpected_list_auctions_target_input(vector<string> arguments, bool is_from_client);
bool is_unexpected_list_auctions_input(vector<string> arguments, bool is_from_client);
bool is_unexpected_list_bids_target_input(vector<string> arguments, bool is_from_client);
bool is_unexpected_show_record_input(vector<string> arguments, bool is_from_client);
bool is_unexpected_close_input(vector<string> arguments, bool is_from_client);
bool is_unexpected_show_asset_input(vector<string> arguments, bool is_from_client);
bool is_unexpected_bid_input(vector<string> arguments, bool is_from_client);
bool is_unexpected_open_input(vector<string> arguments, bool is_from_client);

bool is_unexpected_login_input(vector<string> arguments);
bool is_unexpected_logout_input(vector<string> arguments);
bool is_unexpected_unregister_input(vector<string> arguments);
bool is_unexpected_list_auctions_target_input(vector<string> arguments);
bool is_unexpected_list_auctions_input(vector<string> arguments);
bool is_unexpected_list_bids_target_input(vector<string> arguments);
bool is_unexpected_show_record_input(vector<string> arguments);
bool is_unexpected_close_input(vector<string> arguments);
bool is_unexpected_show_asset_input(vector<string> arguments);
bool is_unexpected_bid_input(vector<string> arguments);
bool is_unexpected_open_input(vector<string> arguments);

bool is_unexpected_login_output(vector<string> arguments, string status);
bool is_unexpected_logout_output(vector<string> arguments, string status);
bool is_unexpected_unregister_output(vector<string> arguments, string status);
bool is_unexpected_open_output(vector<string> arguments, string status);
bool is_unexpected_close_output(vector<string> arguments, string status);
bool is_unexpected_myauctions_output(vector<string> arguments, string status);
bool is_unexpected_mybids_output(vector<string> arguments, string status);
bool is_unexpected_list_output(vector<string> arguments, string status);
bool is_unexpected_bid_output(vector<string> arguments, string status);
bool is_unexpected_show_record_output(vector<string> arguments, string status);
bool is_unexpected_show_asset_output(vector<string> arguments, string status);

#endif
