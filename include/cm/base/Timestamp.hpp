#pragma once

#include <string>

namespace cm {
	class Timestamp {
	public:
		Timestamp() : microSecondsSinceEpoch_(0) {}

		explicit Timestamp(const int64_t microSecondsSinceEpoch) : microSecondsSinceEpoch_(microSecondsSinceEpoch) {}

		static Timestamp now();

		[[nodiscard]] std::string toString() const;

		[[nodiscard]] int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }

		bool operator<(const Timestamp &rhs) const {
			return this->microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
		}

		bool operator==(const Timestamp &rhs) const {
			return this->microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
		}

	private:
		int64_t microSecondsSinceEpoch_;
	};
}