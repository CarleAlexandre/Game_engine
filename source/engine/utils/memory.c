#include <immintrin.h>

void*	memcpy_simd(void *dest, const void *src, size_t n) {
    unsigned char *d = dest;
    const unsigned char *s = src;

    size_t i = 0;
    for (; i + 16 <= n; i += 16) {
        __m128i chunk = _mm_loadu_si128((const __m128i *)(s + i));
        _mm_storeu_si128((__m128i *)(d + i), chunk);
    }
    for (; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

void*	memset_simd(void *dest, int c, size_t n) {
	unsigned char *d = dest;
	size_t i = 0;
    
	__m128i val = _mm_set1_epi8((char)c);
	for (; i + 16 <= n; i += 16) {
	    _mm_storeu_si128((__m128i *)(d + i), val);
	}
	for (; i < n; i++) {
	    d[i] = (unsigned char)c;
	}
	return dest;
}

int	memcmp_simd(const void *s1, const void *s2, size_t n) {
	const unsigned char *a = s1;
	const unsigned char *b = s2;
    
	size_t i = 0;
	for (; i + 16 <= n; i += 16) {
	    __m128i va = _mm_loadu_si128((const __m128i *)(a + i));
	    __m128i vb = _mm_loadu_si128((const __m128i *)(b + i));
	    __m128i cmp = _mm_cmpeq_epi8(va, vb);
	    int mask = _mm_movemask_epi8(cmp);
	    if (mask != 0xFFFF) {
		// not equal somewhere, fallback to scalar check
		for (int j = 0; j < 16; j++) {
		    if (a[i + j] != b[i + j])
			return a[i + j] - b[i + j];
		}
	    }
	}
	for (; i < n; i++) {
	    if (a[i] != b[i])
		return a[i] - b[i];
	}
	return 0;
}

void*	memmove_simd(void *dest, const void *src, size_t n) {
	unsigned char *d = dest;
	const unsigned char *s = src;
    
	if (d < s) {
	    return memcpy_simd(dest, src, n);
	} else {
	    size_t i = n;
	    while (i >= 16) {
		i -= 16;
		__m128i chunk = _mm_loadu_si128((const __m128i *)(s + i));
		_mm_storeu_si128((__m128i *)(d + i), chunk);
	    }
	    while (i-- > 0) {
		d[i] = s[i];
	    }
	    return dest;
	}
}
