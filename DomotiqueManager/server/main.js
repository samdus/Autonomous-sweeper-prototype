import { Meteor } from 'meteor/meteor';
import '../imports/api/tasks.js';
import StatNumberContainer from '../imports/api/statNumberContainer.js';
import StatBoolContainer from '../imports/api/statBoolContainer.js';
import StatStringContainer from '../imports/api/statStringContainer.js';
import ConsoleContainer from '../imports/api/consoleContainer.js';
Meteor.startup(() => {
    StatNumberContainer.StatContainer._collection._ensureIndex({ "history.createdAt": -1});
    StatBoolContainer.StatContainer._collection._ensureIndex({ "history.createdAt": -1});
    StatStringContainer.StatContainer._collection._ensureIndex({ "history.createdAt": -1});
    ConsoleContainer.ConsoleContainer._collection._ensureIndex({ "createdAt": -1});
});
