#ifndef CONSTANTS_H
#define CONSTANTS_H


namespace constants
{

    constexpr double pi{ 3.14159 };
    constexpr double accelerate_g{ 9.81 }; 

	constexpr unsigned int screen_width{ 800 };
	constexpr unsigned int screen_height{ 600 };

	constexpr float radius_of_mouse{ 4.0f };

	struct borderCollision {
		bool top = 0;
		bool bottom = 0;
		bool left = 0;
		bool right = 0;
	};

}
#endif