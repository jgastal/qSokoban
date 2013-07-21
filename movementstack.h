#ifndef MOVEMENTSTACK_H
#define MOVEMENTSTACK_H

#include <QObject>
#include <QStack>

class Level;

class MovementStack : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool canUndo READ canUndo NOTIFY stackChanged)
	Q_PROPERTY(bool canRedo READ canRedo NOTIFY stackChanged)

	public:
		enum Movement {
			STEP_UP,
			STEP_DOWN,
			STEP_LEFT,
			STEP_RIGHT,
			PUSH_UP,
			PUSH_DOWN,
			PUSH_LEFT,
			PUSH_RIGHT
		};
		explicit MovementStack(Level *parent = 0);
		Q_INVOKABLE void undo();
		Q_INVOKABLE void redo();
		Q_INVOKABLE void reset();

	signals:
		void stackChanged();

	protected slots:
		void moved(Movement mv);

	protected:
		bool canUndo() const;
		bool canRedo() const;

	private:
		Level *lvl_;
		QStack<Movement> done_;
		QStack<Movement> undone_;
};

#endif // MOVEMENTSTACK_H
