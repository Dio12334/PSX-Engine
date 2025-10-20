#ifndef UUID_H
#define UUID_H

#include <cstdint>
#include <unordered_map>

namespace psx {
	class UUID{

		public:
			UUID();
			UUID(std::uint64_t uuid);
			UUID(const UUID&) = default;
			
			operator std::uint64_t() const { return m_UUID; }
		private:
			std::uint64_t m_UUID;
	};
}


namespace std {
	template<>
	struct hash<psx::UUID>{
		std::size_t operator()(const psx::UUID& uuid) const {
			return hash<std::uint64_t>()((std::uint64_t)uuid);
		}
	};
}
#endif

