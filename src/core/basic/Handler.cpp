#include "Handler.hpp"

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



void EmptyHandler::setViewport(int width, int height) {
	this->m_operation->setViewport(width, height);
	this->m_operation->operator ()(*this);
}



void RectangleHandler::setViewport(int width, int height) {
	this->m_indicator->setViewport(width, height);
	this->m_operation->setViewport(width, height);
	this->m_operation->operator ()(*this);
}

bool RectangleHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, osg::Object *object, osg::NodeVisitor *visitor) {
	
	osg::Vec2 pos(ea.getX(), ea.getY());
	osg::Vec2 delta = pos - this->m_lastPos;

	if (ea.getModKeyMask() & osgGA::GUIEventAdapter::ModKeyMask::MODKEY_CTRL) {
		switch (ea.getEventType()) {
		case osgGA::GUIEventAdapter::EventType::PUSH:
			this->m_mask = this->computePositionMask(pos);
			this->m_beginPos = pos;
			this->m_lastPos = pos;
			if (this->m_mask == PositionMask::Outside) {
				this->m_start = pos;
				this->m_stop = pos;
			}
			this->m_indicator->setGeometry(this->m_start, this->m_stop);
			break;
		case osgGA::GUIEventAdapter::EventType::RELEASE:
			this->m_mask = PositionMask::None;
			this->m_lastPos = pos;
			this->m_indicator->setGeometry(this->m_start, this->m_stop);
			break;
		case osgGA::GUIEventAdapter::EventType::DRAG:
			this->m_lastPos = pos;
			if (this->m_mask == PositionMask::Outside) {
				this->m_start = osg::Vec2(
						std::min<float>(this->m_beginPos.x(), this->m_lastPos.x()),
						std::min<float>(this->m_beginPos.y(), this->m_lastPos.y()));
				this->m_stop = osg::Vec2(
						std::max<float>(this->m_beginPos.x(), this->m_lastPos.x()),
						std::max<float>(this->m_beginPos.y(), this->m_lastPos.y()));
			} else if (this->m_mask == PositionMask::Center) {
				this->m_start += delta;
				this->m_stop += delta;
			} else {
				if (this->m_mask & PositionMask::Top) this->m_stop.y() += delta.y();
				if (this->m_mask & PositionMask::Bottom) this->m_start.y() += delta.y();
				if (this->m_mask & PositionMask::Right) this->m_stop.x() += delta.x();
				if (this->m_mask & PositionMask::Left) this->m_start.x() += delta.x();
				if (this->m_start.x() > this->m_stop.x()) {
					if (this->m_mask & PositionMask::Right) this->m_start.x() = this->m_stop.x();
					if (this->m_mask & PositionMask::Left) this->m_stop.x() = this->m_start.x();
				}
				if (this->m_start.y() > this->m_stop.y()) {
					if (this->m_mask & PositionMask::Top) this->m_start.y() = this->m_stop.y();
					if (this->m_mask & PositionMask::Bottom) this->m_stop.y() = this->m_start.y();
				}
			}
			this->m_indicator->setGeometry(this->m_start, this->m_stop);
			break;
		default:
			return (false);
		}
		this->m_operation->operator ()(ea, aa, *this);
		return (true);
	}
	return (false);
}

int RectangleHandler::computePositionMask(osg::Vec2 pos) const {
	float x = pos.x(), minX = this->m_start.x(), maxX = this->m_stop.x();
	float y = pos.y(), minY = this->m_start.y(), maxY = this->m_stop.y();
	int result = 0;
	if ((x >= minX) && (x <= maxX) && (y >= minY) && (y <= maxY)) return (PositionMask::Center);
	if ((y > maxY) && (y < maxY + RESPONSIVE_BAND_WIDTH)) result |= PositionMask::Top;
	if ((y < minY) && (y > minY - RESPONSIVE_BAND_WIDTH)) result |= PositionMask::Bottom;
	if ((x > maxX) && (x < maxX + RESPONSIVE_BAND_WIDTH)) result |= PositionMask::Right;
	if ((x < minX) && (x > minX - RESPONSIVE_BAND_WIDTH)) result |= PositionMask::Left;
	if ((x >= maxX + RESPONSIVE_BAND_WIDTH) || (x <= minX - RESPONSIVE_BAND_WIDTH) || (y >= maxY + RESPONSIVE_BAND_WIDTH) || (y <= minY - RESPONSIVE_BAND_WIDTH)) return (PositionMask::Outside);
	return (result);
}



void SegmentHandler::setViewport(int width, int height) {
	this->m_indicator->setViewport(width, height);
	this->m_operation->setViewport(width, height);
	this->m_operation->operator ()(*this);
}

bool SegmentHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, osg::Object *object, osg::NodeVisitor *visitor) {
	
	osg::Vec2 pos(ea.getX(), ea.getY());
	osg::Vec2 delta = pos - this->m_lastPos;

	if (ea.getModKeyMask() & osgGA::GUIEventAdapter::ModKeyMask::MODKEY_ALT) {
		switch (ea.getEventType()) {
		case osgGA::GUIEventAdapter::EventType::PUSH:
			this->m_mask = this->computePositionMask(pos);
			this->m_beginPos = pos;
			this->m_lastPos = pos;
			if (this->m_mask == PositionMask::Outside) {
				this->m_start = pos;
				this->m_stop = pos;
			}
			this->m_indicator->setGeometry(this->m_start, this->m_stop);
			break;
		case osgGA::GUIEventAdapter::EventType::RELEASE:
			this->m_mask = PositionMask::None;
			this->m_lastPos = pos;
			this->m_indicator->setGeometry(this->m_start, this->m_stop);
			return (true);
		case osgGA::GUIEventAdapter::EventType::DRAG:
			this->m_lastPos = pos;
			if (this->m_mask == PositionMask::Outside) {
				this->m_stop = this->m_lastPos;
			} else if (this->m_mask == PositionMask::Midpoint) {
				this->m_start += delta;
				this->m_stop += delta;
			} else if (this->m_mask == PositionMask::Start) {
				this->m_start += delta;
			} else if (this->m_mask == PositionMask::Stop) {
				this->m_stop += delta;
			}
			this->m_indicator->setGeometry(this->m_start, this->m_stop);
			break;
		default:
			return (false);
		}
		this->m_operation->operator ()(ea, aa, *this);
		return (true);
	}
	return (false);
}

int SegmentHandler::computePositionMask(osg::Vec2 pos) const {
	float x = pos.x(), minX = this->m_start.x(), maxX = this->m_stop.x(), midX = minX + (maxX - minX) / 2.0f;
	float y = pos.y(), minY = this->m_start.y(), maxY = this->m_stop.y(), midY = minY + (maxY - minY) / 2.0f;
	if (       (x > minX - RESPONSIVE_BAND_WIDTH)
			&& (x < minX + RESPONSIVE_BAND_WIDTH)
			&& (y > minY - RESPONSIVE_BAND_WIDTH)
			&& (y < minY + RESPONSIVE_BAND_WIDTH)) return (PositionMask::Start);
	if (       (x > maxX - RESPONSIVE_BAND_WIDTH)
			&& (x < maxX + RESPONSIVE_BAND_WIDTH)
			&& (y > maxY - RESPONSIVE_BAND_WIDTH)
			&& (y < maxY + RESPONSIVE_BAND_WIDTH)) return (PositionMask::Stop);
	float a = (pos - this->m_start) * (this->m_stop - this->m_start);
	float b = (pos - this->m_stop) * (this->m_start - this->m_stop);
	if ((a > 0) && (b > 0)) {
		osg::Vec2 direct = this->m_start - this->m_stop;
		osg::Vec2 normal = osg::Vec2(-direct.y(), direct.x());
		normal.normalize();
		float d = std::abs((pos - this->m_start) * normal + (pos - this->m_stop) * normal);
		if (d < RESPONSIVE_BAND_WIDTH) return (PositionMask::Midpoint);
	}
	return (PositionMask::Outside);
}



void PickupHandler::setViewport(int width, int height) {
	this->m_indicator->setViewport(width, height);
	this->m_operation->setViewport(width, height);
	this->m_operation->operator ()(*this);
}

bool PickupHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, osg::Object *object, osg::NodeVisitor *visitor) {
	
	osg::Vec2 pos(ea.getX(), ea.getY());
	
	if (ea.getModKeyMask() & (osgGA::GUIEventAdapter::ModKeyMask::MODKEY_SHIFT | osgGA::GUIEventAdapter::ModKeyMask::MODKEY_ALT)) {
		switch (ea.getEventType()) {
		case osgGA::GUIEventAdapter::EventType::PUSH:
			this->m_beginPos = pos;
			this->m_lastPos = pos;
			this->m_start = pos;
			this->m_stop = pos;
			this->m_indicator->setGeometry(this->m_start, this->m_stop);
			break;
		case osgGA::GUIEventAdapter::EventType::RELEASE:
			this->m_lastPos = pos;
			this->m_indicator->setGeometry(osg::Vec2(0.0f, 0.0f), osg::Vec2(0.0f, 0.0f));
			break;
		case osgGA::GUIEventAdapter::EventType::DRAG:
			this->m_lastPos = pos;
			this->m_start = osg::Vec2(
					std::min<float>(this->m_beginPos.x(), this->m_lastPos.x()),
					std::min<float>(this->m_beginPos.y(), this->m_lastPos.y()));
			this->m_stop = osg::Vec2(
					std::max<float>(this->m_beginPos.x(), this->m_lastPos.x()),
					std::max<float>(this->m_beginPos.y(), this->m_lastPos.y()));
			this->m_indicator->setGeometry(this->m_start, this->m_stop);
			break;
		default:
			return (false);
		}
		this->m_operation->operator ()(ea, aa, *this);
		return (true);
	}
	return (false);
}



#pragma warning (pop)
