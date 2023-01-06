//$file${.::table.cpp} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: dpp.qm
// File:  ${.::table.cpp}
//
// This code has been generated by QM 5.2.4 <www.state-machine.com/qm>.
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// SPDX-License-Identifier: GPL-3.0-or-later
//
// This generated code is open source software: you can redistribute it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation.
//
// This code is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// NOTE:
// Alternatively, this generated code may be distributed under the terms
// of Quantum Leaps commercial licenses, which expressly supersede the GNU
// General Public License and are specifically designed for licensees
// interested in retaining the proprietary status of their code.
//
// Contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//
//$endhead${.::table.cpp} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#include "qpcpp.hpp"
#include "dpp.hpp"
#include "bsp.hpp"

Q_DEFINE_THIS_FILE

// Active object class -------------------------------------------------------
//$declare${AOs::Table} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace DPP {

//${AOs::Table} ..............................................................
class Table : public QP::QActive {
public:
    static Table inst;

private:
    uint8_t m_fork[N_PHILO];
    bool m_isHungry[N_PHILO];

public:
    Table();

protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(active);
    Q_STATE_DECL(serving);
    Q_STATE_DECL(paused);
}; // class Table

} // namespace DPP
//$enddecl${AOs::Table} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

namespace DPP {

// helper function to provide the RIGHT neighbour of a Philo[n]
inline uint8_t RIGHT(uint8_t const n) {
    return static_cast<uint8_t>((n + (N_PHILO - 1U)) % N_PHILO);
}

// helper function to provide the LEFT neighbour of a Philo[n]
inline uint8_t LEFT(uint8_t const n) {
    return static_cast<uint8_t>((n + 1U) % N_PHILO);
}

constexpr uint8_t FREE = static_cast<uint8_t>(0);
constexpr uint8_t USED = static_cast<uint8_t>(1);

constexpr char const * const THINKING = &"thinking"[0];
constexpr char const * const HUNGRY   = &"hungry  "[0];
constexpr char const * const EATING   = &"eating  "[0];

} // namespace DPP

//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 700U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpcpp version 7.0.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$define${AOs::AO_Table} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace DPP {

//${AOs::AO_Table} ...........................................................
QP::QActive * const AO_Table = &Table::inst; // "opaque" pointer to Table AO

} // namespace DPP
//$enddef${AOs::AO_Table} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${AOs::Table} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace DPP {

//${AOs::Table} ..............................................................
Table Table::inst;

//${AOs::Table::Table} .......................................................
Table::Table()
  : QActive(&initial)
{
    for (uint8_t n = 0U; n < N_PHILO; ++n) {
        m_fork[n] = FREE;
        m_isHungry[n] = false;
    }
}

//${AOs::Table::SM} ..........................................................
Q_STATE_DEF(Table, initial) {
    //${AOs::Table::SM::initial}
    (void)e; // suppress the compiler warning about unused parameter

    QS_OBJ_DICTIONARY(&Table::inst);

    QS_SIG_DICTIONARY(DONE_SIG,      nullptr); // global signals
    QS_SIG_DICTIONARY(EAT_SIG,       nullptr);
    QS_SIG_DICTIONARY(PAUSE_SIG,     nullptr);
    QS_SIG_DICTIONARY(SERVE_SIG,     nullptr);
    QS_SIG_DICTIONARY(TEST_SIG,      nullptr);

    QS_SIG_DICTIONARY(HUNGRY_SIG,    this); // signal just for Table

    subscribe(DONE_SIG);
    subscribe(PAUSE_SIG);
    subscribe(SERVE_SIG);
    subscribe(TEST_SIG);

    for (uint8_t n = 0U; n < N_PHILO; ++n) {
        m_fork[n] = FREE;
        m_isHungry[n] = false;
        BSP::displayPhilStat(n, THINKING);
    }

    QS_FUN_DICTIONARY(&Table::active);
    QS_FUN_DICTIONARY(&Table::serving);
    QS_FUN_DICTIONARY(&Table::paused);

    return tran(&serving);
}

//${AOs::Table::SM::active} ..................................................
Q_STATE_DEF(Table, active) {
    QP::QState status_;
    switch (e->sig) {
        //${AOs::Table::SM::active::TEST}
        case TEST_SIG: {
            status_ = Q_RET_HANDLED;
            break;
        }
        //${AOs::Table::SM::active::EAT}
        case EAT_SIG: {
            Q_ERROR();
            status_ = Q_RET_HANDLED;
            break;
        }
        default: {
            status_ = super(&top);
            break;
        }
    }
    return status_;
}

//${AOs::Table::SM::active::serving} .........................................
Q_STATE_DEF(Table, serving) {
    QP::QState status_;
    switch (e->sig) {
        //${AOs::Table::SM::active::serving}
        case Q_ENTRY_SIG: {
            for (uint8_t n = 0U; n < N_PHILO; ++n) { // give permissions to eat...
                if (m_isHungry[n]
                    && (m_fork[LEFT(n)] == FREE)
                    && (m_fork[n] == FREE))
                {
                    m_fork[LEFT(n)] = USED;
                    m_fork[n] = USED;
                    TableEvt *te = Q_NEW(TableEvt, EAT_SIG);
                    te->philoNum = n;
                    QP::QActive::PUBLISH(te, this);
                    m_isHungry[n] = false;
                    BSP::displayPhilStat(n, EATING);
                }
            }
            status_ = Q_RET_HANDLED;
            break;
        }
        //${AOs::Table::SM::active::serving::HUNGRY}
        case HUNGRY_SIG: {
            uint8_t n = Q_EVT_CAST(TableEvt)->philoNum;
            // phil ID must be in range and he must be not hungry
            Q_ASSERT((n < N_PHILO) && (!m_isHungry[n]));

            BSP::displayPhilStat(n, HUNGRY);
            uint8_t m = LEFT(n);
            //${AOs::Table::SM::active::serving::HUNGRY::[bothfree]}
            if ((m_fork[m] == FREE) && (m_fork[n] == FREE)) {
                m_fork[m] = USED;
                m_fork[n] = USED;
                TableEvt *pe = Q_NEW(TableEvt, EAT_SIG);
                pe->philoNum = n;
                QP::QActive::PUBLISH(pe, this);
                BSP::displayPhilStat(n, EATING);
                status_ = Q_RET_HANDLED;
            }
            //${AOs::Table::SM::active::serving::HUNGRY::[else]}
            else {
                m_isHungry[n] = true;
                status_ = Q_RET_HANDLED;
            }
            break;
        }
        //${AOs::Table::SM::active::serving::DONE}
        case DONE_SIG: {
            uint8_t n = Q_EVT_CAST(TableEvt)->philoNum;
            // phil ID must be in range and he must be not hungry
            Q_ASSERT((n < N_PHILO) && (!m_isHungry[n]));

            BSP::displayPhilStat(n, THINKING);
            uint8_t m = LEFT(n);
            // both forks of Phil[n] must be used
            Q_ASSERT((m_fork[n] == USED) && (m_fork[m] == USED));

            m_fork[m] = FREE;
            m_fork[n] = FREE;
            m = RIGHT(n); // check the right neighbor

            if (m_isHungry[m] && (m_fork[m] == FREE)) {
                m_fork[n] = USED;
                m_fork[m] = USED;
                m_isHungry[m] = false;
                TableEvt *pe = Q_NEW(TableEvt, EAT_SIG);
                pe->philoNum = m;
                QP::QActive::PUBLISH(pe, this);
                BSP::displayPhilStat(m, EATING);
            }
            m = LEFT(n); // check the left neighbor
            n = LEFT(m); // left fork of the left neighbor
            if (m_isHungry[m] && (m_fork[n] == FREE)) {
                m_fork[m] = USED;
                m_fork[n] = USED;
                m_isHungry[m] = false;
                TableEvt *pe = Q_NEW(TableEvt, EAT_SIG);
                pe->philoNum = m;
                QP::QActive::PUBLISH(pe, this);
                BSP::displayPhilStat(m, EATING);
            }
            status_ = Q_RET_HANDLED;
            break;
        }
        //${AOs::Table::SM::active::serving::EAT}
        case EAT_SIG: {
            Q_ERROR();
            status_ = Q_RET_HANDLED;
            break;
        }
        //${AOs::Table::SM::active::serving::PAUSE}
        case PAUSE_SIG: {
            status_ = tran(&paused);
            break;
        }
        default: {
            status_ = super(&active);
            break;
        }
    }
    return status_;
}

//${AOs::Table::SM::active::paused} ..........................................
Q_STATE_DEF(Table, paused) {
    QP::QState status_;
    switch (e->sig) {
        //${AOs::Table::SM::active::paused}
        case Q_ENTRY_SIG: {
            BSP::displayPaused(1U);
            status_ = Q_RET_HANDLED;
            break;
        }
        //${AOs::Table::SM::active::paused}
        case Q_EXIT_SIG: {
            BSP::displayPaused(0U);
            status_ = Q_RET_HANDLED;
            break;
        }
        //${AOs::Table::SM::active::paused::SERVE}
        case SERVE_SIG: {
            status_ = tran(&serving);
            break;
        }
        //${AOs::Table::SM::active::paused::HUNGRY}
        case HUNGRY_SIG: {
            uint8_t n = Q_EVT_CAST(TableEvt)->philoNum;
            // philo ID must be in range and he must be not hungry
            Q_ASSERT((n < N_PHILO) && (!m_isHungry[n]));
            m_isHungry[n] = true;
            BSP::displayPhilStat(n, HUNGRY);
            status_ = Q_RET_HANDLED;
            break;
        }
        //${AOs::Table::SM::active::paused::DONE}
        case DONE_SIG: {
            uint8_t n = Q_EVT_CAST(TableEvt)->philoNum;
            // phil ID must be in range and he must be not hungry
            Q_ASSERT((n < N_PHILO) && (!m_isHungry[n]));

            BSP::displayPhilStat(n, THINKING);
            uint8_t m = LEFT(n);
            /* both forks of Phil[n] must be used */
            Q_ASSERT((m_fork[n] == USED) && (m_fork[m] == USED));

            m_fork[m] = FREE;
            m_fork[n] = FREE;
            status_ = Q_RET_HANDLED;
            break;
        }
        default: {
            status_ = super(&active);
            break;
        }
    }
    return status_;
}

} // namespace DPP
//$enddef${AOs::Table} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
