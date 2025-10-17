#pragma once

namespace sensors
{
	class ISensor
	{
	public:
		virtual ~ISensor() = default;
		virtual bool IsMeasurementValid() const = 0;

		virtual const std::string& GetTimestamp() const = 0;
	};
}
