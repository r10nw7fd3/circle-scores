#include <cstddef>

size_t uleb128_encode(unsigned long long val, char* dest, size_t dest_size) {
	size_t size;

	for(size = 0; val && size < dest_size; ++size) {
		dest[size] = val & 127;
		val >>= 7;
		if(val)
			dest[size] |= 128;
	}
	return size;
}
