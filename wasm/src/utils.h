#pragma once

#include "yet_another_enums_header_file.h" // I was tired when I created those files =_=
#include "yet_another_math_header_file.h"

struct StringUtils {
	/**
	 * Transform a string to lowercase.
	 */
	static std::string to_lower(const std::string& s) {
		std::string result = s;
		std::transform(result.begin(), result.end(), result.begin(), ::tolower);
		return result;
	}

	/**
	 * Transform a string to uppercase.
	 */
	static std::string to_upper(const std::string& s) {
		std::string result = s;
		std::transform(result.begin(), result.end(), result.begin(), ::toupper);
		return result;
	}

	/**
	 * Split a string into a vector of strings.
	 */
	static std::vector<std::string> split(const std::string& s, char delimiter) {
		std::vector<std::string> tokens;
		std::string              token;
		std::istringstream       token_stream(s);
		while (std::getline(token_stream, token, delimiter)) {
			tokens.push_back(token);
		}
		return tokens;
	}
};