#include "Pch.h"


void merge(PFDV *a, PFDV *b, int l, int m, int r)
{
	int h, i, j, k;
	h = l;
	i = l;
	j = m + 1;

	while ((h <= m) && (j <= r)) {
		if (a[h] <= a[j]) {
			b[i] = a[h];
			h++;
		}

		else {
			b[i] = a[j];
			j++;
		}

		i++;
	}

	if (m < h) {
		for (k = j; k <= r; k++) {
			b[i] = a[k];
			i++;
		}
	}

	else {
		for (k = h; k <= m; k++) {
			b[i] = a[k];
			i++;
		}
	}

	for (k = l; k <= r; k++) {
		a[k] = b[k];
	}
}

void mergeSort(PFDV *a, PFDV *b, int l, int r)
{
	int m;

	if (l < r) {
		m = (l + r) / 2;

		mergeSort(a, b, l, m);
		mergeSort(a, b, (m + 1), r);
		merge(a, b, l, m, r);
	}
}
