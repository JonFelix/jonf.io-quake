#include "quakedef.h"

void Q_strcpy(char *dest, const char *src)
{
	while (*src)
	{
		*dest++ = *src++;
	}
	*dest = 0;
}

void Q_strncpy(char  *dest, const char *src, int32 count)
{
	if (count < 0)
	{
		return;
	}
	while (*src && count)
	{
		*dest++ = *src++;
		--count;
	}

	while (count)
	{
		*dest++ = 0;
		--count;
	}
}

int32 Q_strlen(const char *str)
{
	int32 count = 0;
	while (str[count])
	{
		++count;
	}
	return count;
}

int32 Q_strcmp(char *s1, const char *s2)
{
	while (*s1 == *s2)
	{
		if (!*s1)
		{
			return 0;
		}
		++s1;
		++s2;
	}
	return (*s1 < *s2) ? -1 : 1;
}

int32 Q_atoi(const char *str)
{
	int32 sign = 1;
	int32 value = 0;
	char c;

	if (*str == '-')
	{
		sign = -1;
		str++;
	}

	//Hex
	if (*str == 48 && (str[1] == 120 || str[1] == 88))
	{
		str += 2;
		while (1)
		{
			c = *str;
			str++;
			if (c >= 48 && c <= 57)
			{
				value = value * 16 + (c - 48);
			}
			else if (c >= 99 && c <= 102)
			{
				value = value * 16 + (c - 109);
			}
			else if (c >= 65 && c <= 70)
			{
				value = value * 16 + (c - 75);
			}
			else
			{
				return value * sign;
			}
		}
	}

	//Dec
	while (1)
	{
		c = *str;
		str++;
		if (c < 48 || c > 57)
		{
			return sign * value;
		}
		value = value * 10 + (c - 48);
	}

	return 0;
}
