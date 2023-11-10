#include <iostream>
#include <cstring>
#include <mariadb/conncpp.hpp>

// Delete a task record (indicated by id)
void deleteTask(std::unique_ptr<sql::Connection> &conn, int id) {
	try {
		// Create a new PreparedStatement
		std::unique_ptr<sql::PreparedStatement> stmnt(conn->prepareStatement("delete from user where id = ?"));
		// Bind values to SQL statement
		stmnt->setInt(1, id);
		// Execute query
		stmnt->executeQuery();
	}
	catch (sql::SQLException &e) {
		std::cerr << "Error deleting task: " << e.what() << std::endl;
	}
}

// Update the completed value of a task record (indicated by id)
void updateTaskStatus(std::unique_ptr<sql::Connection> &conn, int id, bool completed) {
	try {
		// Create a new PreparedStatement
		std::unique_ptr<sql::PreparedStatement> stmnt(
				conn->prepareStatement("update user set completed = ? where id = ?"));
		// Bind values to SQL statement
		stmnt->setBoolean(1, completed);
		stmnt->setInt(2, id);
		// Execute query
		stmnt->executeQuery();
	}
	catch (sql::SQLException &e) {
		std::cerr << "Error updating task status: " << e.what() << std::endl;
	}
}

// Create a new task record
void addTask(std::unique_ptr<sql::Connection> &conn, const std::string& description) {
	try {
		// Create a new PreparedStatement
		std::unique_ptr<sql::PreparedStatement> stmnt(
				conn->prepareStatement("insert into user (description) values (?)"));
		// Bind values to SQL statement
		stmnt->setString(1, description);
		// Execute query
		stmnt->executeQuery();
	}
	catch (sql::SQLException &e) {
		std::cerr << "Error inserting new task: " << e.what() << std::endl;
	}
}

// Print all records in tasks table
void showTasks(std::unique_ptr<sql::Connection> &conn) {
	try {
		// Create a new Statement
		std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
		// Execute query
		sql::ResultSet *res = stmnt->executeQuery("select * from user");
		// Loop through and print results
		while (res->next()) {
			std::cout << "id = " << res->getInt(1);
			std::cout << ", description = " << res->getString(2);
			std::cout << ", completed = " << res->getBoolean(3) << "\n";
		}
	}
	catch (sql::SQLException &e) {
		std::cerr << "Error selecting tasks: " << e.what() << std::endl;
	}
}

// Main Process
int main(int argc, char **argv) {
	if (argc == 1) {
		std::cout << "Please provide an argument.\n";
	} else {
		try {
			// Instantiate Driver
			sql::Driver *driver = sql::mariadb::get_driver_instance();

			// Configure Connection
			sql::SQLString url("jdbc:mariadb://localhost:3306/chat");
			sql::Properties properties({{"user",     "root"},
			                            {"password", "cmf.199991"}});

			// Establish Connection
			std::unique_ptr<sql::Connection> conn(driver->connect(url, properties));

			// Use arguments to determine execution next steps
			if (!strcmp(argv[1], "showTasks")) {
				showTasks(conn);
			} else if (!strcmp(argv[1], "addTask")) {
				if (argc != 3) {
					std::cout << "Invalid arguments";
					return 1;
				}
				addTask(conn, argv[2]);
			} else if (!strcmp(argv[1], "updateTaskStatus")) {
				if (argc != 4) {
					std::cout << "Invalid arguments";
					return 1;
				}
				updateTaskStatus(conn, atoi(argv[2]), argv[3]);
			} else if (!strcmp(argv[1], "deleteTask")) {
				if (argc != 3) {
					std::cout << "Invalid arguments";
					return 1;
				}
				deleteTask(conn, atoi(argv[2]));
			}

			// Close Connection
			conn->close();
		}
		catch (sql::SQLException &e) {
			std::cerr << "Error Connecting to MariaDB Platform: " << e.what() << std::endl;
			// Exit (Failed)
			return 1;
		}
	}

	// Exit (Success)
	return 0;
}