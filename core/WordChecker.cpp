// WordChecker.cpp
//
// ICS 46 Spring 2018
// Project #4: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"
#include <algorithm>


WordChecker::WordChecker(const Set<std::string>& words)
	: words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
	return words.contains(word);
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
	std::vector< std::string > sugg_vec;
	int s_len = word.length();
	std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	//adjacent switch
	for(int i = 0; i < s_len - 1; i++)
	{
		std::string adj_s = word;
		std::swap(adj_s[i], adj_s[i + 1]);
		if(words.contains(adj_s) && std::find(sugg_vec.begin(), sugg_vec.end(), adj_s) == sugg_vec.end())
			sugg_vec.push_back(adj_s);
	}

	//char adding
	for(char c: alphabet)
	{
		for(int i = 0; i <= s_len; i++)
		{
			std::string add_s = word;
			add_s.insert(i, 1, c);
			if(words.contains(add_s) && std::find(sugg_vec.begin(), sugg_vec.end(), add_s) == sugg_vec.end())
				sugg_vec.push_back(add_s);

		}
	}

	//char deleting
	for(int i = 0; i < s_len; i++)
	{
		std::string del_s = word;
		del_s.erase(i, 1);
		if(words.contains(del_s) && std::find(sugg_vec.begin(), sugg_vec.end(), del_s) == sugg_vec.end())
			sugg_vec.push_back(del_s);
	}

	//char replacing
	for(char c : alphabet)
	{
		for(int i = 0; i < s_len; i++)
		{
			std::string replace_s = word;
			replace_s.replace(i, 1, 1, c);
			if(words.contains(replace_s) && std::find(sugg_vec.begin(), sugg_vec.end(), replace_s) == sugg_vec.end())
				sugg_vec.push_back(replace_s);
		}
	}

	//char spacing
	for(int i = 0; i < s_len; i++)
	{
		std::string space_s = word;
		space_s.insert(i, 1, ' ');
		if(words.contains(space_s) && std::find(sugg_vec.begin(), sugg_vec.end(), space_s) == sugg_vec.end())
			sugg_vec.push_back(space_s);
	}

	return sugg_vec;
}

