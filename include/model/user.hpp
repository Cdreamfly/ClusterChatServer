#pragma once

#include <string>

class User {
public:
	User() = default;

	User(int id, std::string_view &, std::string_view &, std::string_view = "offline");

	virtual ~User() = default;

	void setId(int);

	void setName(const std::string_view &);

	void setPwd(const std::string_view &);

	void setState(const std::string_view &);

	[[nodiscard]] int getId() const;

	[[nodiscard]] std::string_view getName() const;

	[[nodiscard]] std::string_view getPwd() const;

	[[nodiscard]] std::string_view getState() const;

private:
	int id_{};
	std::string name_{};
	std::string pwd_{};
	std::string state_{};
};