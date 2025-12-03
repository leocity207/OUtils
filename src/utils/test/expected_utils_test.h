#ifndef SRC_UTILS_TEST_H
#define SRC_UTILS_TEST_H

#include <string>
#include <utility>

struct TrackValue
{
	int v;
	static inline int constructions = 0;
	static inline int copies = 0;
	static inline int moves = 0;
	static inline int destructions = 0;

	static void Reset() {
		constructions = copies = moves = destructions = 0;
	}

	TrackValue(int x = 0) : v(x) { ++constructions; }
	TrackValue(const TrackValue& other) : v(other.v) { ++copies; }
	TrackValue(TrackValue&& other) noexcept : v(other.v) { ++moves; other.v = -1; }
	~TrackValue() { ++destructions; }

	bool operator==(const TrackValue& o) const { return v == o.v; }
};

struct TrackError
{
	std::string msg;
	static inline int constructions = 0;
	static inline int copies = 0;
	static inline int moves = 0;
	static inline int destructions = 0;

	static void Reset() {
		constructions = copies = moves = destructions = 0;
	}

	TrackError() : msg() { ++constructions; }
	explicit TrackError(std::string m) : msg(std::move(m)) { ++constructions; }
	TrackError(const TrackError& o) : msg(o.msg) { ++copies; }
	TrackError(TrackError&& o) noexcept : msg(std::move(o.msg)) { ++moves; }
	~TrackError() { ++destructions; }

	bool operator==(const TrackError& o) const { return msg == o.msg; }
};

#endif //SRC_UTILS_TEST_H